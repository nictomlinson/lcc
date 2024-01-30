#include "split16.md.pre.c"
static int splitCost(int spaceCost, int timeCost){
  // TODO: allow selection of optimsation criteria via a commandline switch
return spaceCost;
}

// Get the offset from the frame pointer for a local
// and formal parameter
static int frameOffset(Node a){
  assert(a->syms[0]->scope >= PARAM && a->syms[0]->sclass != STATIC && "only expected for locals and formals");
  return a->syms[0]->x.offset;
  }
static int cvSrcSize(Node a) { return a->syms[0]->u.c.v.i; }

static int ifCost(int cond, int spaceCost, int timeCost){
  return cond ?  splitCost(spaceCost, timeCost) : LBURG_MAX;
}

static int inRange(Node a, int lo, int hi) {
  if (range(a, lo, hi) == 0) return 1;
  else
    return 0;
}

static int between(int a, int lo, int hi) { return (a >= lo && a <= hi); }

static int rootNode(Node a) { return !(a->x.prev); }
static int variadicNode(Node a) { return variadic(a->syms[0]->type); }

static int argNo(Node a) { return a->x.argno; }

static void emitSymbol(Symbol p, const char *qualifier, int endLine) {
  if (!verbose) return;
  
  print("%I.info", indent);
  if (*qualifier) print(" %s", qualifier);
  print(" name=%s, type=%t, src=%w", p->name, p->type, &p->src);

  switch (p->scope) {
    case CONSTANTS:
      print(" scope=CONSTANT");
      break;
    case LABELS:
      print(" scope=LABEL");
      break;
    case GLOBAL:
      print(" scope=GLOBAL");
      break;
    case PARAM:
      print(" scope=PARAM");
      break;
    case LOCAL:
      print(" scope=%s", p->temporary ? "TEMP" : "LOCAL");
      break;
    default:
      if (p->scope > LOCAL)
        print(" scope=%s+%d", p->temporary ? "TEMP" : "LOCAL",
              p->scope - LOCAL);
      else
        print(" scope=SCOPE_%d", p->scope);
  }
  print(" sclass=%k", p->sclass);
  print(" size=%d", p->type->size);
  print(" flags=");
  {
    int n = 0;
#define yy(f)            \
  if (p->f) {            \
    if (n++) print("|"); \
    print(#f);           \
  }
    // clang-format off
    yy(structarg)
    yy(addressed)
    yy(computed)
    yy(temporary)
    yy(generated)
    if (n == 0) print("none");
    // clang-format on
#undef yy
  }
  if (p->scope >= PARAM && p->sclass != STATIC) {
    print(" offset=%d", p->x.offset);
  }
  print(" ref=%f", p->ref);
  if(endLine)
    print("\n");
}

static void I(segment)(int n) {
  if (cseg != n) switch (cseg = n) {
      case CODE:
        print("\n.segment code\n");
        return;
      case DATA:
        print("\n.segment data\n");
        return;
      case BSS:
        print("\n.segment bss\n");
        return;
      case LIT:
        print("\n.segment lit\n");
        return;
      default:
        assert(0 && "unknown segment");
    }
}

static void dumptree(Node p) {
  switch (specific(p->op)) {
    case ASGN + B:
      assert(p->kids[0]);
      assert(p->kids[1]);
      assert(p->syms[0]);
      indent++;
      dumptree(p->kids[0]);
      dumptree(p->kids[1]);
      indent--;
      print("%I%s %d\n", indent, opname(p->op), p->syms[0]->u.c.v.u);
      return;
    case RET + V:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      print("%I%s\n", indent, opname(p->op));
      return;
  }
  switch (generic(p->op)) {
    case CNST:
    case ADDRG:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%I%s %s // offset=%d\n", indent, opname(p->op), p->syms[0]->x.name,
            p->syms[0]->x.offset);
      return;
    case ADDRF:
    case ADDRL:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%I%s %d // name=%s\n", indent, opname(p->op), p->syms[0]->x.offset,
            p->syms[0]->x.name);
      return;
    case LABEL:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%ILABEL %s: // offset=%d\n", indent, p->syms[0]->x.name,
            p->syms[0]->x.offset);
      return;
    case CVF:
    case CVI:
    case CVP:
    case CVU:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0]);
      indent++;
      dumptree(p->kids[0]);
      indent--;
      {
        const char *op = opname(p->op);
        int srcSize = p->syms[0]->u.c.v.i;
        assert(strlen(op) == 5 && "Expected opname for CVx to be 5 long");
        print("%I%S%d%s\n", indent, op, 3, srcSize, op + 3);
      }
      return;
    case ARG:
    case BCOM:
    case NEG:
    case INDIR:
    case JUMP:
    case RET:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      indent++;
      dumptree(p->kids[0]);
      indent--;
      print("%I%s\n", indent, opname(p->op));
      if (generic(p->op) == ARG) {
        int argSize = roundup(p->syms[0]->u.c.v.i, 2) / 2;
        callArgCells += argSize;
      }
      return;
    case CALL:
      assert(p->kids[0]);
      if(specific(p->op) != CALL+B) assert(!p->kids[1]);
      if(specific(p->op) == CALL+B) assert(p->kids[1]);
      assert(p->syms[0]);
        // Other calls, through a function pointer need to push
        // the address on the stack and do an indirect call
        indent++;
        dumptree(p->kids[0]);
        if (specific(p->op) == CALL + B) dumptree(p->kids[1]);
        indent--;
        print("%I%s", indent, opname(p->op));
      if (variadic(p->syms[0]->type))
        print(" %d // variadic call\n", callArgCells);
      else
        print("\n");
      callArgCells = 0;
      return;
    case ASGN:
      assert(p->kids[0]);
      assert(p->kids[1]);

      indent++;
      dumptree(p->kids[0]);
      dumptree(p->kids[1]);
      indent--;
      print("%I%s\n", indent, opname(p->op));
      return;
    case BOR:
    case BAND:
    case BXOR:
    case RSH:
    case LSH:
    case ADD:
    case SUB:
    case DIV:
    case MUL:
    case MOD:
      assert(p->kids[0]);
      assert(p->kids[1]);
      indent++;
      dumptree(p->kids[0]);
      dumptree(p->kids[1]);
      indent--;
      print("%I%s\n", indent, opname(p->op));
      return;
    case EQ:
    case NE:
    case GT:
    case GE:
    case LE:
    case LT:
      assert(p->kids[0]);
      assert(p->kids[1]);
      assert(p->syms[0]);
      assert(p->syms[0]->x.name);
      indent++;
      dumptree(p->kids[0]);
      dumptree(p->kids[1]);
      indent--;
      print("%I%s %s\n", indent, opname(p->op), p->syms[0]->x.name);
      return;
  }
  assert(0);
}

static int getrule(Node p, int nt) {
  int rulenum=0;
  
  assert(p);
  // implement the same assertion that _rule will implement on nt
  // so that we can dump the node in error before _rule causes a
  // a fatal error that kills the process. NELEMS(_ntname) is just
  // a convient and reliable way to get the same upper limit that
  // _rule uses in its assertion. In _rule, it is generated by lburg
  if (!(nt < 1 || nt > NELEMS(_ntname)-2)) rulenum = _rule(p->x.state, nt);
  if (!rulenum) {
    fprint(stderr, "(%x->op=%s at %w is corrupt.)\n", p, opname(p->op), &src);
    print("Node in error>>>\n");
    dumptree(p);
    print("<<<Node in error\n");
    fflush(stdout);
    assert(0);
  }
  return rulenum;
}

static void emit2(Node p) { assert(0 && "Not expected emit2 to be used"); }

static int asmLinePos = 0;
static void emitAsmAdvanceTo(int pos){
    do {
      putchar(' ');
      asmLinePos += 1;
    } while (asmLinePos < pos);
}
static void emitAsmCh(char c){
  if (c == ';') emitAsmAdvanceTo(40);
  (void)putchar(c);
  asmLinePos++;
  if (c == '\n') asmLinePos = 0;

}
static void emitAsmStr(char *s){
  int i;
  for (i = 0; s[i]; i++) {
    emitAsmCh(s[i]);
  }
}

static unsigned emitassembly(Node p, int nt) {
  int rulenum;
  short *nts;
  char *fmt;
  Node kids[10];

  rulenum = getrule(p, nt);
  nts = _nts[rulenum];
  fmt = _templates[rulenum];
  assert(fmt);
  if (_isinstruction[rulenum] && p->x.emitted)
    print("%s ; NOT EXPECTING THIS", p->syms[RX]->x.name);
  else if (*fmt == '#')
    emit2(p);
  else {
    if (*fmt == '?' && (generic(p->op)==ARG || generic(p->op) == CALL )){
      if( p->x.argno !=0) {
        // skip the first assembly instruction (to newline) if this is an
        // an ARG Node and the argno is not zero or this is a CALL with no
        // arguments. Used to ensure the push of the stack pointer only happens
        // on the first arg
        // The standard code generator uses this "?" technique to discard moves
        // from a register and itself 
        fmt++;
          while (*fmt++ != '\n')
          ;
      } else {
        fmt++;
      }
    }
    for (_kids(p, rulenum, kids); *fmt; fmt++)
      if (*fmt != '%'){
        emitAsmCh(*fmt);
      } else if (*++fmt == 'F')
        print("%d", framesize);
      else if (*fmt >= '0' && *fmt <= '9'){
        indent++;
        emitassembly(kids[*fmt - '0'], nts[*fmt - '0']);
        indent--;
      }
      else if (*fmt >= 'a' && *fmt < 'a' + NELEMS(p->syms)){
        emitAsmStr(p->syms[*fmt - 'a']->x.name);
      }
      /*      else if (*fmt >= 'A' && *fmt < 'A' + NELEMS(p->syms))
                print("%d", p->syms[*fmt - 'A']->x.offset);*/
      else if (*fmt == 'L'){
        print("\n%I", indent);
        asmLinePos = indent;
      } else if (*fmt == 'I'){
        print("%I", indent);
        asmLinePos += indent;
      } else if (*fmt == 'T') {
        emitAsmAdvanceTo(24);
      } else if (*fmt == '<') indent -= 1;
      else if (*fmt == '>') indent +=1;
      else{
        emitAsmCh(*fmt);
      }
    }
    return 0;
}

static void I(emitBurg)(Node p) {
  for (; p; p = p->x.next) {
#if(0)
    assert(p->x.registered);
#endif
    if (!(generic(p->op) == LABEL)) indent += 2;
    emitassembly(p, p->x.inst);
    p->x.emitted = 1;
    if (!(generic(p->op) == LABEL)) indent -= 2;
    //  if(!(generic(p->op)==LABEL))
    //    print("\n");
  }
}


static Node genDiscard(Node p) {
  Node newNode;
  if (generic(p->op) == CALL && optype(p->op) != VOID
       && specific(p->op) != CALL + B) {
    assert(specific(p->op) != CALL + B && "not expecting CALLB ");
    newNode = newnode(POP + optype(p->op) + sizeop(opsize(p->op)), p, NULL,
                      intconst(roundup(opsize(p->op), 2)));
    newNode->link = p->link;
    p->link = NULL;
    return newNode;
  }
  return p;
}

static void reduce(Node p, int nt) {
	int rulenum, i;
	short *nts;
	Node kids[10];

	rulenum = getrule(p, nt);
	nts = _nts[rulenum];
	_kids(p, rulenum, kids);
	for (i = 0; nts[i]; i++)
		reduce(kids[i], nts[i]);
	if (_isinstruction[rulenum]) {
		assert(p->x.inst == 0 || p->x.inst == nt);
		p->x.inst = nt;
		if (p->syms[RX] && p->syms[RX]->temporary) {
			debug(fprint(stderr, "(using %s)\n", p->syms[RX]->name));
			p->syms[RX]->x.usecount++;
		}
	}
}

static Node *prune(Node p, Node pp[]) {
	if (p == NULL)
		return pp;
	p->x.kids[0] = p->x.kids[1] = p->x.kids[2] = NULL;
	if (p->x.inst == 0)
		return prune(p->kids[1], prune(p->kids[0], pp));
	else {
		prune(p->kids[1], prune(p->kids[0], &p->x.kids[0]));
		*pp = p;
		return pp + 1;
	}
}

static void linearize(Node p, Node next) {
	int i;

	for (i = 0; i < NELEMS(p->x.kids) && p->x.kids[i]; i++)
		linearize(p->x.kids[i], next);
	relink(next->x.prev, p);
	relink(p, next);
	debug(fprint(stderr, "(listing %x)\n", p));
}

static int doArg_argno = 0;
static int doArg_callNesting = 0;
static void doarg(Node p) {
    p->x.argno=doArg_argno++;
    if(p->x.argno == 0){
      doArg_callNesting += 1;
    }
}

static void docall(Node p) {
  if(doArg_argno>0)
    assert(doArg_callNesting == 1 &&
          "Don't support nesting of calls since need to track first arg");
  if(doArg_argno==0)
    assert(doArg_callNesting == 0 &&
          "Don't support nesting of calls since need to track first arg");
    p->x.argno = doArg_argno;  // Save number of arguments for a call
  if(doArg_argno!=0){
    doArg_argno = 0;
    doArg_callNesting -= 1;
  }
}

static Node I(gen)(Node forest) {
  Node q;
  Node dummy;
  Node prev= NULL;
  struct node sentinel;

  assert(forest);

  for (q = forest; q; q = q->link) {
     q = genDiscard(q);
     if(prev){
       prev->link = q;
     }else{
       forest = q; // in case we replaced the start of the forest
     }
    if (generic(q->op) == CALL)
      docall(q);
    else if (   generic(q->op) == ASGN
            && generic(q->kids[1]->op) == CALL)
      docall(q->kids[1]);
    else if (   generic(q->op)==POP
            && generic(q->kids[0]->op) == CALL)
     docall(q->kids[0]);
    else if (generic(q->op) == ARG)
      (*IR->x.doarg)(q);

     _label(q);
     reduce(q, 1);
     prev = q;
  }
  for (q = forest; q; q = q->link) {
    prune(forest, &dummy);
  }

  relink(&sentinel, &sentinel);
  for (q = forest; q; q = q->link){
    linearize(q, &sentinel);
  }

  forest = sentinel.x.next;
  assert(forest);
  sentinel.x.next->x.prev = NULL;
  sentinel.x.prev->x.next = NULL;

  return forest;
}

static void I(progbeg)(int argc, char *argv[]) {
  {
    union {
      char c;
      int i;
    } u;
    u.i = 0;
    u.c = 1;
    swap = ((int)(u.i == 1)) != IR->little_endian;
  }
  int i;
  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-v") == 0)
      verbose = 1;
  indent = 0;
  print("; Split16/std Assembly generated by LCC4.2\n");
}

static void I(progend)(void) {}

static void I(local)(Symbol p) {
  p->sclass = AUTO;
  mkauto(p);
  // offset = roundup(offset, p->type->align);
  // p->x.name = p->name;
  // // Frame point points at the first argument, locals are below FP
  // p->x.offset = -2-offset;
  // offset += p->type->size;
  emitSymbol(p, "local", 1);
}

static void I(function)(Symbol f, Symbol caller[], Symbol callee[],
                        int ncalls) {
  int i;
  int returnValueSize = f->type->type->size;

  (*IR->segment)(CODE);
  if(f->sclass != STATIC)
    print("%I.export %s: {\n", indent++, f->x.name);
  else
    print("%I %s: {\n", indent++, f->x.name);

  if(verbose){
    emitSymbol(f, "function", 0);
    print(" nrCalls=%d\n", ncalls);
    if (!hasproto(f->type)) print("%I.info NoPrototype\n", indent);
  }

  offset = 0;
  for (i = 0; caller[i] && callee[i]; i++) {
    caller[i]->x.offset = callee[i]->x.offset = offset;
    caller[i]->x.name = callee[i]->x.name = stringd(caller[i]->x.offset);
    caller[i]->sclass = callee[i]->sclass = AUTO;
    offset += roundup(caller[i]->type->size, 2);
  }

  maxargoffset = maxoffset = argoffset = offset = 0;
  gencode(caller, callee);
  framesize = roundup(maxoffset, 2);

  if(verbose){
    for (i = 0; callee[i]; i++) {
      emitSymbol(callee[i], "callee", 1);
    }
    print("%I.info returnSize=%d maxoffset=%d maxargoffset=%d isVariadc=%s\n", indent, 
        f->type->type->size, maxoffset, maxargoffset, variadic(f->type) ? "yes" : "no");
  }
  // emit prolog
  // The caller placed the old FP and SP on the return stack, as well as the
  // return address, before setting FP and SP to point at the first paramater on
  // the stack. We now need to allocate space for locals and, if this function
  // makes any calls, save our FP to the data stack
  indent += 2;
  if(ncalls==0){
    if (framesize<256) {
      print("%IenterLeaf.8 %d\n", indent, framesize);
    } else {
      print("%IenterLeaf.16 %d\n", indent, framesize);
    }
  } else {
    if (framesize<256) {
      print("%Ienter.8 %d\n", indent, framesize);
    } else {
      print("%Ienter.16 %d\n", indent, framesize);
    }
  }

  callArgCells = 0;
  emitcode();

  // emit epilog.
  if(ncalls==0)
    print("%IexitLeaf\n", indent);
  else
    print("%Iexit\n", indent);
  indent -= 2;
  print("%I}\n", --indent);
}

static void I(defsymbol)(Symbol p) {
  if (p->scope >= LOCAL && p->sclass == STATIC)
    p->x.name = stringf("L_%d", genlabel(1));
  else if (p->generated)
    p->x.name = stringf("L_%s", p->name);
  else if (p->scope == GLOBAL || p->sclass == EXTERN)
    p->x.name = stringf("_%s", p->name);
  else
    p->x.name = p->name;
}


static void I(address)(Symbol q, Symbol p, long n) {
  if (p->scope == GLOBAL || p->sclass == STATIC || p->sclass == EXTERN)
    q->x.name = stringf("%s%s%D", p->x.name, n >= 0 ? "+" : "", n);
  else {
    assert(n <= INT_MAX && n >= INT_MIN);
    q->x.offset = p->x.offset + n;
    q->x.name = stringd(q->x.offset);
  }
}

static void I(defconst)(int suffix, int size, Value v) {
  if (suffix == I && size == 1)
    print("%I.def1b 0x%X\n", indent, v.i & 0xff);
  else if (suffix == I && size == 2)
    print("%I.def2b 0x%X\n", indent, v.i & 0xffff);
  else if (suffix == I && size == 4)
    print("%I.def4b 0x%X\n", indent, v.i & 0xffffffff);
  else if (suffix == U && size == 1)
    print("%I.def1b 0x%X\n", indent, v.u & 0xff);
  else if (suffix == U && size == 2)
    print("%I.def2b 0x%X\n", indent, v.u & 0xffff);
  else if (suffix == U && size == 4)
    print("%I.def4b 0x%X\n", indent, v.u & 0xffffffff);
  else if (suffix == P && size == 2)
    print("%I.def2b 0x%X\n", indent, ((unsigned)v.p) & 0xffff);
  else if (suffix == F && size == 4) {
    float f = v.d;
    print(".def4b 0x%X\n", (*(unsigned *)&f) & 0xffffffff);
  } else if (suffix == F && size == 8) {
    double d = v.d;
    unsigned *p = (unsigned *)&d;
    print("%I.def8b 0x%X\n.def4b 0x%X\n", indent, p[swap] & 0xffffffff,
          p[!swap] & 0xffffffff);
  } else
    assert(0);
}

static void I(defaddress)(Symbol p) { print(".def2b %s\n", p->x.name); }

static void I(defstring)(int len, char *str) {
  char *s;
  for (s = str; s < str + len; s++) print(".def1b %d\n", (*s) & 0xFF);
}

static void I(export)(Symbol p) {
  if (p->scope != GLOBAL)
    print("%I.export %s //TODO: not expecting to emit this\n", indent,
          p->x.name);
}

static void I(import)(Symbol p) { print("%I.extern %s\n", indent, p->x.name); }

static void I(global)(Symbol p) {
  assert(p->type->align == 1 || p->type->align == 2 && "unexpected alignment");
  print(".align %d\n", p->type->align);
  if (p->sclass != STATIC) print(".export ");
  print("%s:\n", p->x.name);
  if (p->u.seg == BSS) print("	.defs %d\n", p->type->size);
}

static void I(space)(int n) {
  if (cseg != BSS)
    print("%I.skip %d // seg%d\n", indent, n, cseg);
  else
    print("%I.info nospace for BSS segment\n", indent);
}

static void I(stabline)(Coordinate *cp) {
  static char *prevfile;
  static int prevline;

  if (cp->file && (prevfile == NULL || strcmp(prevfile, cp->file) != 0)) {
    print("file \"%s\"\n", prevfile = cp->file);
    prevline = 0;
  }
  if (cp->y != prevline) print("line %d\n", prevline = cp->y);
}

#define split16_blockbeg blockbeg
#define split16_blockend blockend

// clang-format off
Interface split16IR = {
    /* size, align, outofline */
    1, 1, 0, /* char */
    2, 2, 0, /* short */
    2, 2, 0, /* int */
    4, 2, 0, /* long */
    4, 2, 0, /* long long */
    4, 2, 1, /* float */
    8, 2, 1, /* double */
    8, 2, 0, /* long double */
    2, 2, 0, /* T* */
    0, 2, 0, /* struct */
    0, /* little_endian */
    0, /* mulops_calls */
    0, /* wants_callb */
    1, /* wants_argb */
    0, /* left_to_right */
    0, /* wants_dag */
    0, /* unsigned_char */
    I(address),
    I(blockbeg),
    I(blockend),
    I(defaddress),
    I(defconst),
    I(defstring),
    I(defsymbol),
    I(emitBurg),
    I(export),
    I(function),
    I(gen),
    I(global),
    I(import),
    I(local),
    I(progbeg),
    I(progend),
    I(segment),
    I(space),
    0, /* I(stabblock) */
    0, /* I(stabend) */
    0, /* I(stabfend) */
    0, /* I(stabinit) */
    I(stabline),
    0, /* I(stabsym) */
    0, /* I(stabtype) */

{
	0, /* max_unaligned_load */
	0, /* rmap */
	0, /* blkfetch */
	0, /* blkstore */
	0, /* blkloop */
	0, /* _label */
	0, /* _rule */
	0, /* _nts */
	0, /* _kids */
	0, /* _string */
	0, /* _templates */
	0, /* _isinstruction */
	0, /* _ntname */
	0, /* emit2 */
	doarg, /* doarg */
	0, /* target  */
	0, /* clobber  */
}
};
// clang-format on
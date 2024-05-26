#include "split16.md.pre.c"
static int genCost(int spaceCost, int timeCost) {
  // TODO: allow selection of optimsation criteria via a commandline switch
  return (spaceCost * 100) + timeCost;
}
/* Get the size of the source for a conversion node */
static int cvSrcSize(Node a) { return a->syms[0]->u.c.v.i; }

static int isFunction(Node a) {
  assert(generic(a->op) == ADDRG);
  return a->syms[0]->type && isfunc(a->syms[0]->type);
}

/* Determine if the function being called has the __fcall attribute */
static int isFcall(Node a) {
  assert(generic(a->op) == CALL);
  return a->syms[0]->type && isfunc(a->syms[0]->type) &&
         a->syms[0]->type->u.f.fcall;
}

static int isNotFunction(Node a) {
  assert(generic(a->op) == ADDRG);
  return a->syms[0]->type && !isfunc(a->syms[0]->type);
}

static int isSeg(Node a, int seg) {
  assert(generic(a->op) == ADDRG);
  return a->syms[0] && a->syms[0]->u.seg == seg;
}

static int ifCost(int cond, int spaceCost, int timeCost) {
  return cond ? genCost(spaceCost, timeCost) : LBURG_MAX;
}

static int inRange(Node a, int lo, int hi) {
  if (range(a, lo, hi) == 0)
    return 1;
  else
    return 0;
}

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
  if (endLine) print("\n");
}

static void I(segment)(int n) {
  if (cseg != n) switch (cseg = n) {
      case CODE:
        print("\n.section text\n");
        return;
      case DATA:
        print("\n.section data\n");
        return;
      case BSS:
        print("\n.section bss\n");
        return;
      case LIT:
        print("\n.section lit\n");
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
      /*      assert(!p->kids[1]);*/
      indent++;
      dumptree(p->kids[0]);
      indent--;
      print("%I%s\n", indent, opname(p->op));
      return;
    case CALL:
      assert(p->kids[0]);
      if (specific(p->op) != CALL + B) assert(!p->kids[1]);
      if (specific(p->op) == CALL + B) assert(p->kids[1]);
      assert(p->syms[0]);
      // Other calls, through a function pointer need to push
      // the address on the stack and do an indirect call
      indent++;
      dumptree(p->kids[0]);
      if (specific(p->op) == CALL + B) dumptree(p->kids[1]);
      indent--;
      print("%I%s\n", indent, opname(p->op));
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
  print("%I,Should not be here, op:%x, %s\n", indent, p->op, opname(p->op));
  /* assert(0); */
}

static int getrule(Node p, int nt) {
  int rulenum = 0;

  assert(p);
  // implement the same assertion that _rule will implement on nt
  // so that we can dump the node in error before _rule causes a
  // a fatal error that kills the process. NELEMS(_ntname) is just
  // a convient and reliable way to get the same upper limit that
  // _rule uses in its assertion. In _rule, it is generated by lburg
  if (!(nt < 1 || nt > NELEMS(_ntname) - 2)) rulenum = _rule(p->x.state, nt);
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

static int asmLinePos = 1;
static void emitAsmAdvanceTo(int tabStop);
// Emit a single character
static void emitAsmCh(char c) {
  if (c == ';') emitAsmAdvanceTo(4);
  (void)putchar(c);
  if (c == '\t')
    asmLinePos = (((asmLinePos - 1) >> 3) << 3) + 8 + 1;
  else
    asmLinePos++;
  if (c == '\n') {
    asmLinePos = 1;
  }
}

static void emitAsmAdvanceTo(int tabStop) {
  emitAsmCh('\t');
  while (asmLinePos < (tabStop * 8 + 1)) {
    emitAsmCh('\t');
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
    if (*fmt == '?' && (generic(p->op) == ARG || generic(p->op) == CALL)) {
      if (p->x.argno != 0) {
        // skip the first assembly instruction (up to the first newline) if this
        // is an
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
      if (*fmt != '%') {
        emitAsmCh(*fmt);
      } else if (*++fmt == 'F') {
        print("%d", framesize);
      } else if (*fmt >= '0' && *fmt <= '9') {
        indent++;
        emitassembly(kids[*fmt - '0'], nts[*fmt - '0']);
        indent--;
      } else if (*fmt >= 'a' && *fmt < 'a' + NELEMS(p->syms)) {
        int i;
        char *s = p->syms[*fmt - 'a']->x.name;
        for (i = 0; s[i]; i++) {
          emitAsmCh(s[i]);
        }
      } else if (*fmt == 'L') {
        print("\n\t%I", indent);
        asmLinePos = indent + 1;
      } else if (*fmt == 'I') {
        print("\t%I", indent);
        asmLinePos += indent;
      } else if (*fmt == 'T') {
        emitAsmAdvanceTo(2);
      } else if (*fmt == '<')
        indent -= 1;
      else if (*fmt == '>')
        indent += 1;
      else {
        emitAsmCh(*fmt);
      }
  }
  return 0;
}

static void I(emitBurg)(Node p) {
  for (; p; p = p->x.next) {
#if (0)
    assert(p->x.registered);
#endif
    emitassembly(p, p->x.inst);
    p->x.emitted = 1;
  }
}

static Node genDiscard(Node p) {
  Node newNode;
  if (generic(p->op) == CALL && optype(p->op) != VOID &&
      specific(p->op) != CALL + B) {
    assert(specific(p->op) != CALL + B && "not expecting CALLB ");
    newNode = newnode(POP + optype(p->op) + sizeop(opsize(p->op)), p, NULL,
                      intconst(roundup(opsize(p->op), 2)));
    newNode->link = p->link;
    p->link = NULL;
    return newNode;
  }
  return p;
}
/* check if P or its descendents are a CALL*/
static Node findCall(Node p) {
  Node call = NULL;
  if (p->kids[0])
    if ((call = findCall(p->kids[0])) != NULL) return call;
  if (p->kids[1])
    if ((call = findCall(p->kids[1])) != NULL) return call;
  if (generic(p->op) == CALL) return p;
  return NULL;
}

static Node genStartArg(Node q) {
  Node call = NULL, p, newNode, prev;

  if (generic(q->op) == CALL || generic(q->op) == ARG) {
    if (q->x.argno != 0) return q;
  } else if (generic(q->op) == ASGN && generic(q->kids[1]->op) == CALL) {
    if (q->kids[1]->x.argno != 0) return q;
  } else if (generic(q->op) == POP && generic(q->kids[0]->op) == CALL) {
    if (q->kids[0]->x.argno != 0) return q;
  } else
    return q;

  for (p = q; p; p = p->link) {
    call = findCall(p);
    if (call != NULL) break;
  }
  if (call == NULL) {
    error("call not found for ARG or CALL\n");
    return q;
  }
  if(call->syms[1] == NULL || call->syms[1]->type == NULL){
    error("No type in sysm[1] for call\n");
    return q;
  }
  if( !isfunc(call->syms[1]->type)){
    error("Type found for callee of call is not a function: %t\n",
          call->syms[1]->type);
    return q;
  }
  if(verbose)
    print(".info Found callee type: %t\n", call->syms[1]->type);

  if (call->syms[1]->type->u.f.fcall == 1) {
    /* Don't save sp if the calling convention of callee is fcall */
    return q;
  }
  newNode = newnode(ARGSTART, NULL, NULL, NULL);
  newNode->link = q;
  return newNode;
}

static void reduce(Node p, int nt) {
  int rulenum, i;
  short *nts;
  Node kids[10];

  rulenum = getrule(p, nt);
  nts = _nts[rulenum];
  _kids(p, rulenum, kids);
  for (i = 0; nts[i]; i++) reduce(kids[i], nts[i]);
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
  if (p == NULL) return pp;
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

static int argno;
static void doarg(Node p) {
  int align = 2;

  if (argoffset == 0) argno = 0;
  p->x.argno = argno++;
  p->syms[2] = intconst(mkactual(align, p->syms[0]->u.c.v.i));
}

static void docall(Node p) {
  p->syms[1] = p->syms[0];
  p->syms[0] = intconst(argoffset);
  if (argoffset > maxargoffset) maxargoffset = argoffset;
  p->x.argno = argno;
  argoffset = 0;
}

static Node I(gen)(Node forest) {
  Node q;
  Node dummy;
  Node prev;
  struct node sentinel;

  assert(forest);

  for (prev = NULL, q = forest; q; q = q->link) {
    q = genDiscard(q);
    if (prev) {
      prev->link = q;
    } else {
      forest = q;  // in case we replaced the start of the forest
    }
    if (generic(q->op) == CALL)
      docall(q);
    else if (generic(q->op) == ASGN && generic(q->kids[1]->op) == CALL)
      docall(q->kids[1]);
    else if (generic(q->op) == POP && generic(q->kids[0]->op) == CALL)
      docall(q->kids[0]);
    else if (generic(q->op) == ARG)
      (*IR->x.doarg)(q);

    prev = q;
  }

  for (prev = NULL, q = forest; q; q = q->link) {
    Node newNode = genStartArg(q);
    if (prev) {
      prev->link = newNode;
    } else {
      forest = newNode;  // in case we replaced the start of the forest
    }
    prev = q;
  }

  for (q = forest; q; q = q->link) {
    _label(q);
    reduce(q, 1);
  }

  for (q = forest; q; q = q->link) {
    prune(forest, &dummy);
  }

  relink(&sentinel, &sentinel);
  for (q = forest; q; q = q->link) {
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
    swap = ((int)(u.i == 1)) != IR->little_endian; /*not used?*/
    hostLittle = ((int)(u.i == 1));
  }
  int i;
  for (i = 1; i < argc; i++)
    if (strcmp(argv[i], "-v") == 0) verbose = 1;
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

  (*IR->segment)(CODE);
  print("%s: {\n", f->x.name);

  if (verbose) {
    emitSymbol(f, "function", 0);
    print(" nrCalls=%d\n", ncalls);
    if (!hasproto(f->type)) print(".info NoPrototype\n");
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

  if (verbose) {
    for (i = 0; callee[i]; i++) {
      emitSymbol(callee[i], "callee", 1);
    }
    print(".info returnSize=%d maxoffset=%d maxargoffset=%d isVariadc=%s\n",
          f->type->type->size, maxoffset, maxargoffset,
          variadic(f->type) ? "yes" : "no");
  }
  // emit prolog
  // The caller placed the old FP and SP on the return stack, as well as the
  // return address, before setting FP and SP to point at the first paramater on
  // the stack. We now need to allocate space for locals and, if this function
  // makes any calls, save our FP to the data stack
  if (ncalls == 0) {
    print("\tenterLeaf %d\n", framesize);
  } else {
    print("\tenter %d\n", framesize);
  }

  emitcode();

  // emit epilog.
  if (ncalls == 0)
    print("\texitLeaf\n");
  else
    print("\texit\n");
  print("}\n");
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

static void sprintHex(unsigned char *src, int len, unsigned char *buf) {
  int i;
  for (i = hostLittle ? len - 1 : 0; len > 0; i += hostLittle ? -1 : 1, len--) {
    sprintf(buf, "%02X", src[i]);
    buf += 2;
  }
}

static void I(defconst)(int suffix, int size, Value v) {
  unsigned char buf[17];
  unsigned char *p;
  float f = v.d;
  double d = v.d;
  if (suffix == I && (size == 1 || size == 2 || size == 4 || size == 8))
    p = (unsigned char *)&v.i;
  else if (suffix == U && (size == 1 || size == 2 || size == 4 || size == 8))
    p = (unsigned char *)&v.u;
  else if (suffix == P && size == 2)
    p = (unsigned char *)&v.p;
  else if (suffix == F && size == 4)
    p = (unsigned char *)&f;
  else if (suffix == F && size == 8)
    p = (unsigned char *)&d;
  else
    assert(0);
  sprintHex(p, size, buf);
  print("\t.def%d\t0x%s\n", size, buf);
}

static void I(defaddress)(Symbol p) { print("\t.def2b %s\n", p->x.name); }

static void I(defstring)(int len, char *str) {
  char *s;
  for (s = str; s < str + len; s++) print("\t.def1b\t%d\n", (*s) & 0xFF);
}

static void I(export)(Symbol p) { print(".global %s\n", p->x.name); }

static void I(import)(Symbol p) {
  if (isfunc(p->type))
    print(".externC %s\n", p->x.name);
  else
    print(".externD %s\n", p->x.name);
}

static void I(global)(Symbol p) {
  if (p->type->align != 1) print(".align %d\n", p->type->align);
  print("%s:\n", p->x.name);
  if (p->u.seg == BSS) print("\t.defspace %d\n", p->type->size);
}

static void I(space)(int n) {
  if (cseg != BSS) print("\t.defs\t%d\n", n);
}

static void I(stabline)(Coordinate *cp) {
  static char *prevfile;
  static int prevline;

  if (cp->file && (prevfile == NULL || strcmp(prevfile, cp->file) != 0)) {
    print(".file \"%s\"\n", prevfile = cp->file);
    prevline = 0;
  }
  if (cp->y != prevline) print(".line %d\n", prevline = cp->y);
}

#define split16_blockbeg blockbeg
#define split16_blockend blockend

// clang-format off
Interface split16IR = {
    /* size, align, outofline */
    1, 1, 0, /* char */
    2, 2, 0, /* short */
    2, 2, 0, /* int */
    4, 2, 1, /* long */
    8, 2, 1, /* long long */
    4, 2, 1, /* float */
    8, 2, 1, /* double */
    8, 2, 1, /* long double */
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
    0, /* I(stabline),*/
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

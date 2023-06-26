#include "c.h"
#define I(f) forth_##f

static char rcsid[] = "$Id$";

static int indent;
static int cseg = 0;  // current segment
static int callArgCells;
static int dumpPacked = 0;

static char *suffixes[] = {"0", "F", "D",  "C",  "S",  "I",  "U",  "P",
                           "V", "B", "10", "11", "12", "13", "14", "15"};

static void emitSymbol(Symbol p, const char *qualifier) {
  switch (p->scope) {
    case CONSTANTS:
      print("%I.info CONSTANTS", indent);
      break;
    case LABELS:
      print("%I.info LABELS", indent);
      break;
    case GLOBAL:
      print("%I.info GLOBAL", indent);
      break;
    case PARAM:
      print("%I.info PARAM", indent);
      break;
    case LOCAL:
      print("%I.info %s", indent, p->temporary ? "TEMP" : "LOCAL");
      break;
    default:
      if (p->scope > LOCAL)
        print("%I.info %s+%d", indent, p->temporary ? "TEMP" : "LOCAL",
              p->scope - LOCAL);
      else
        print("%I.indo SCOPE_%d", indent, p->scope);
  }
  if (*qualifier) print("_%s", qualifier);
  /*if (verbose && (src.y || src.x)) emitCoord(p->src);*/
  print(" name=%s", p->name);
  print(" type=%t", p->type);
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
  if (p->temporary && p->u.t.cse) {
    #if(0)
    print(" u.t.cse=%p", p->u.t.cse); // the Node that calculates the temporary
    #endif
  }
  print("\n");
}

static void I(segment)(int n) {
  if (cseg != n) switch (cseg = n) {
      case CODE:
        print("\n.code\n");
        return;
      case DATA:
        print("\n.data\n");
        return;
      case BSS:
        print("\n.bss\n");
        return;
      case LIT:
        print("\n.lit\n");
        return;
      default:
        assert(0 && "unknown segment");
    }
}


static void dumpPackedTree(Node p) {
  switch (specific(p->op)) {
    case ASGN + B:
      assert(p->kids[0]);
      assert(p->kids[1]);
      assert(p->syms[0]);
      indent++;
      dumpPackedTree(p->kids[0]);
      dumpPackedTree(p->kids[1]);
      indent--;
      print("%s.", opname(p->op));
      return;
    case RET + V:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      print("%s.", opname(p->op));
      return;
  }
  switch (generic(p->op)) {
    case CNST:
    case ADDRG:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%s.", opname(p->op));
      return;
    case ADDRF:
    case ADDRL:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%s.", opname(p->op));
      return;
    case LABEL:
      assert(!p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0] && p->syms[0]->x.name);
      print("%s:", p->syms[0]->x.name);
      return;
    case CVF:
    case CVI:
    case CVP:
    case CVU:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0]);
      indent++;
      dumpPackedTree(p->kids[0]);
      indent--;
      {
        const char *op = opname(p->op);
        int srcSize = p->syms[0]->u.c.v.i;
        assert(strlen(op) == 5 && "Expected opname for CVx to be 5 long");
        print("%S%d%s.", op, 3, srcSize, op + 3);
      }
      return;
    case ARG:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      indent++;
      dumpPackedTree(p->kids[0]);
      indent--;
      int argSize = roundup(p->syms[0]->u.c.v.i, 2) / 2;
      callArgCells += argSize;
      return;
    case BCOM:
    case NEG:
    case INDIR:
    case JUMP:
    case RET:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      indent++;
      dumpPackedTree(p->kids[0]);
      indent--;
      print("%s.", opname(p->op));
      return;
    case CALL:
      assert(p->kids[0]);
      assert(!p->kids[1]);
      assert(p->syms[0]);
      assert(optype(p->op) != B);
      if (generic(p->kids[0]->op) == ADDRG) {
        // compile time calls simply only need the
        // address of the target in thread
        Node q = p->kids[0];
        assert(q->syms[0] && q->syms[0]->x.name);
        if (variadic(p->syms[0]->type))
          print("%I%s %d\n", indent, q->syms[0]->x.name, callArgCells);
        else
          print("%I%s\n", indent, q->syms[0]->x.name);
      } else {
        // Other calls, through a function pointer need to push
        // the address on the stack and do an indirect call
        indent++;
        dumpPackedTree(p->kids[0]);
        indent--;
        print("%s.", opname(p->op));
      }
      callArgCells = 0;
      return;
    case ASGN:
      assert(p->kids[0]);
      assert(p->kids[1]);

      indent++;
      dumpPackedTree(p->kids[0]);
      dumpPackedTree(p->kids[1]);
      indent--;
      print("%s.", opname(p->op));
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
      dumpPackedTree(p->kids[0]);
      dumpPackedTree(p->kids[1]);
      indent--;
      print("%s.", opname(p->op));
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
      dumpPackedTree(p->kids[0]);
      dumpPackedTree(p->kids[1]);
      indent--;
      print("%s.", opname(p->op));
      return;
  }
  assert(0);
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
      print("%I%s: // offset=%d\n", indent, p->syms[0]->x.name,
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
      assert(!p->kids[1]);
      assert(p->syms[0]);
      assert(optype(p->op) != B);
      if (generic(p->kids[0]->op) == ADDRG) {
        // compile time calls simply only need the
        // address of the target in thread
        Node q = p->kids[0];
        assert(q->syms[0] && q->syms[0]->x.name);
        print("%I%s", indent, q->syms[0]->x.name);
      } else {
        // Other calls, through a function pointer need to push
        // the address on the stack and do an indirect call
        indent++;
        dumptree(p->kids[0]);
        indent--;
        print("%I%s", indent, opname(p->op));
      }
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

static void I(emit)(Node p) {
  for (; p; p = p->link) {
    if(dumpPacked){
    print("\n%I", indent);
    dumpPackedTree(p);
    } else{
    dumptree(p);
    }
    if (generic(p->op) == CALL && optype(p->op) != VOID) {
      /*credit: S. Woutersen, Building a C-based processor, December, 2005 */
      print("%IDISCARD%s%d\n", indent, suffixes[optype(p->op)], opsize(p->op));
    }
  }
}

static void gen02(Node p) {
  assert(p);
  if (generic(p->op) == ARG) {
    assert(p->syms[0]);
    argoffset += (p->syms[0]->u.c.v.i < 4 ? 4 : p->syms[0]->u.c.v.i);
  } else if (generic(p->op) == CALL) {
    maxargoffset = (argoffset > maxargoffset ? argoffset : maxargoffset);
    argoffset = 0;
  }
}

static void gen01(Node p) {
  if (p) {
    gen01(p->kids[0]);
    gen01(p->kids[1]);
    gen02(p);
  }
}

static Node I(gen)(Node p) {
  Node q;

  assert(p);
  for (q = p; q; q = q->link) {
    gen01(q);
  }
  return p;
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
  indent = 0;
  print("; Split16/Forth Assembly generated by LCC4.2\n");
}

static void I(progend)(void) {}

static void I(local)(Symbol p) {
  offset = roundup(offset, p->type->align);
  p->x.name = p->name;
  p->x.offset = offset;
  offset += p->type->size;
  emitSymbol(p, "");
}

static void I(function)(Symbol f, Symbol caller[], Symbol callee[],
                        int ncalls) {
  int i;
  int cellInCnt = 0;
  (*IR->segment)(CODE);
  print("%I%s %s: {\n", ++indent, f->sclass != STATIC ? ".export" : "", f->x.name);
  if (!hasproto(f->type)) print("%I.info NoPrototype\n", indent);
  offset = 0;
  for (i = 0; caller[i] && callee[i]; i++) {
    caller[i]->x.name = callee[i]->x.name = caller[i]->name;
    caller[i]->x.offset = callee[i]->x.offset = offset;
    caller[i]->sclass = callee[i]->sclass = AUTO;
    offset += roundup(caller[i]->type->size, 2);
  }
  cellInCnt = offset / 2;
  maxargoffset = maxoffset = argoffset = offset = 0;
  gencode(caller, callee);
  print("%I.info maxoffset=%d maxargoffset=%d isVariadc=%s\n", indent, maxoffset,
        maxargoffset, variadic(f->type) ? "yes" : "no");
  // now we know how much local and temp space is used, adjust
  // the parameter offsets as they sit above the locals & temps
  for (i = 0; callee[i]; i++) {
    callee[i]->x.offset += maxoffset;
    emitSymbol(caller[i], "caller");
    emitSymbol(callee[i], "callee");
  }

  // emit prolog
  // Calls to variadic functions are followed by a count in the
  // the thread of the number of cells passed, push this to the
  // stack before we enter
  if (variadic(f->type)) {
    print("%Iimm_ir // variadic cell count\n", indent);
    print("%Ienter\n", indent);
  } else {
    if (cellInCnt) {
      print("%Ienter$ %d\n", indent, cellInCnt);
    } else {
      print("%Ienter\n", indent);
    }
  }
  print("%I.{\n", indent);
  indent += 2;
  if ((cellInCnt || variadic(f->type)) && maxoffset) {
    print("%Iunpack.Using@ %d\n", indent, maxoffset);
  } else if (cellInCnt || variadic(f->type)) {
    print("%Iunpack\n", indent);
  } else if (maxoffset) {
    print("%Iusing@ %d\n", indent, maxoffset);
  }

  callArgCells = 0;
  emitcode();
  print("\n");

  // emit epilog.
  // if the prolog was "enter" then "exit_next" has nothing to clear up
  // otherwise, leave clears up the s/w stack using info left by the prolog
  // TODO: how or where do we put info for leave
  if (cellInCnt || variadic(f->type) || maxoffset)
    print("%Ileave\n", indent);
  else
    print("%IEXIT_NXT\n", indent);
  indent -= 2;
  print("%I.}\n", indent);
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
    print("%I.skip %d\n", indent, n);
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

#define forth_blockbeg blockbeg
#define forth_blockend blockend

// clang-format off
Interface split4IR = {
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
    0, /* wants_argb */
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
    I(emit),
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
};
// clang-format on
%{
#define INTTMP 0xffffffff
#define INTVAR 0xffffffff
#define FLTTMP 0xffffffff
#define FLTVAR 0xffffffff

#define INTRET 0xffffffff
#define FLTRET 0xffffffff
#define REG4 IREG
#define REG8 FREG

#define COST_OF_REG_COPY 2
#define COST_OF_DRG_COPY 4

#include "c.h"

#define readsreg(p) \
        (generic((p)->op)==INDIR && (p)->kids[0]->op==VREG+P)
#define setsrc(d) ((d) && (d)->x.regnode && \
        (d)->x.regnode->set == src->x.regnode->set && \
        (d)->x.regnode->mask&src->x.regnode->mask)

#define relink(a, b) ((b)->x.prev = (a), (a)->x.next = (b))

// Some macros for deciding which rule to use for call node code generation
// Intrinsics are adam standard instructions that behave like functions.
// They take arguments from the stack. Unlike standard calling convention C functions,
// they consume their arguments and so after executing them there is no need to pop
// the arguments from the stack
// The compiler allows these intrinsics to appear in C code like normal extern functions
// and maps them to the corresponding assembler instruction
// We make it the job of the assembler to strip off, or ignore, the leading prefix.
static const char prefix[] = "__admStdIntrinsic_";
static const int prefixLen = strlen(prefix);
#define isNameIntrinsic(f)  ( (strlen(f)>prefixLen && memcmp(f, prefix, prefixLen)==0) )
#define isNodeIntrinsic(p) (p->kids[0] && p->kids[0]->syms[0] && isNameIntrinsic(p->kids[0]->syms[0]->name))
#define callNodeHasArgs(p) (p->syms[0] && p->syms[0]->u.c.v.i > 0)

#define hasargs(p) (!isNodeIntrinsic(p) && callNodeHasArgs(p) ? 0 : LBURG_MAX)
#define hasNoArgs(p) (!isNodeIntrinsic(p) && !callNodeHasArgs(p) ? 0 : LBURG_MAX)
#define onlyIfIntrinsic(p) (isNodeIntrinsic(p) ? 0 : LBURG_MAX) 


#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)
static void address(Symbol, Symbol, long);
static void blkfetch(int, int, int, int);
static void blkloop(int, int, int, int, int, int[]);
static void blkstore(int, int, int, int);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void doarg(Node);
static void emit2(Node);
static void export(Symbol);
static void clobber(Node);
static void function(Symbol, Symbol [], Symbol [], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char **);
static void progend(void);
static void segment(int);
static void space(int);
static void target(Node);
static int      bitcount       (unsigned);
static Symbol reg32[32], reg64[32], d6;
static Symbol reg32wildcard, reg64wildcard; 
static int tmpregs[] = {3, 9, 10};
static Symbol blkreg;

static int gnum = 8;
static int pic;

static int cseg;
%}
%start stmt

%term CNSTF4=4113
%term CNSTI1=1045 CNSTI2=2069 CNSTI4=4117
%term CNSTP4=4119
%term CNSTU1=1046 CNSTU2=2070 CNSTU4=4118
%term ARGB=41
%term ARGF4=4129
%term ARGI4=4133
%term ARGP4=4135
%term ARGU4=4134
%term ASGNB=57
%term ASGNF4=4145
%term ASGNI1=1077 ASGNI2=2101 ASGNI4=4149
%term ASGNP4=4151
%term ASGNU1=1078 ASGNU2=2102 ASGNU4=4150
%term INDIRB=73
%term INDIRF4=4161
%term INDIRI1=1093 INDIRI2=2117 INDIRI4=4165
%term INDIRP4=4167
%term INDIRU1=1094 INDIRU2=2118 INDIRU4=4166
%term CVFF4=4209
%term CVFI4=4213
%term CVIF4=4225
%term CVII1=1157 CVII2=2181 CVII4=4229
%term CVIU1=1158 CVIU2=2182 CVIU4=4230
%term CVPU4=4246
%term CVUI1=1205 CVUI2=2229 CVUI4=4277
%term CVUP4=4279
%term CVUU1=1206 CVUU2=2230 CVUU4=4278
%term NEGF4=4289
%term NEGI4=4293
%term CALLB=217
%term CALLF4=4305
%term CALLI4=4309
%term CALLP4=4311
%term CALLU4=4310
%term CALLV=216
%term RETF4=4337
%term RETI4=4341
%term RETP4=4343
%term RETU4=4342
%term RETV=248
%term ADDRGP4=4359
%term ADDRFP4=4375
%term ADDRLP4=4391
%term ADDF4=4401
%term ADDI4=4405
%term ADDP4=4407
%term ADDU4=4406
%term SUBF4=4417
%term SUBI4=4421
%term SUBP4=4423
%term SUBU4=4422
%term LSHI4=4437
%term LSHU4=4438
%term MODI4=4453
%term MODU4=4454
%term RSHI4=4469
%term RSHU4=4470
%term BANDI4=4485
%term BANDU4=4486
%term BCOMI4=4501
%term BCOMU4=4502
%term BORI4=4517
%term BORU4=4518
%term BXORI4=4533
%term BXORU4=4534
%term DIVF4=4545
%term DIVI4=4549
%term DIVU4=4550
%term MULF4=4561
%term MULI4=4565
%term MULU4=4566
%term EQF4=4577
%term EQI4=4581
%term EQU4=4582
%term GEF4=4593
%term GEI4=4597
%term GEU4=4598
%term GTF4=4609
%term GTI4=4613
%term GTU4=4614
%term LEF4=4625
%term LEI4=4629
%term LEU4=4630
%term LTF4=4641
%term LTI4=4645
%term LTU4=4646
%term NEF4=4657
%term NEI4=4661
%term NEU4=4662
%term JUMPV=584
%term LABELV=600

%term LOADB=233
%term LOADF4=4321
%term LOADI1=1253
%term LOADI2=2277
%term LOADI4=4325
%term LOADP4=4327
%term LOADU1=1254
%term LOADU2=2278
%term LOADU4=4326

%term VREGP=711
%%
reg:  INDIRI1(VREGP)           "# read register\n"
reg:  INDIRU1(VREGP)           "# read register\n"
reg:  INDIRI2(VREGP)           "# read register\n"
reg:  INDIRU2(VREGP)           "# read register\n"
reg:  INDIRF4(VREGP)           "# read register\n"
reg:  INDIRI4(VREGP)           "# read register\n"
reg:  INDIRP4(VREGP)           "# read register\n"
reg:  INDIRU4(VREGP)           "# read register\n"
stmt: ASGNI1(VREGP,reg)        "# write register\n"
stmt: ASGNU1(VREGP,reg)        "# write register\n"
stmt: ASGNI2(VREGP,reg)        "# write register\n"
stmt: ASGNU2(VREGP,reg)        "# write register\n"
stmt: ASGNF4(VREGP,reg)        "# write register\n"
stmt: ASGNI4(VREGP,reg)        "# write register\n"
stmt: ASGNP4(VREGP,reg)        "# write register\n"
stmt: ASGNU4(VREGP,reg)        "# write register\n"
con:   CNSTI1                  "%a"                                0
con:   CNSTU1                  "%a"                                0
con:   CNSTI2                  "%a"                                0
con:   CNSTU2                  "%a"                                0
con:   CNSTI4                  "%a"                                0
con:   CNSTU4                  "%a"                                0
con:   CNSTP4                  "%a"                                0
con:   CNSTF4                  "%a.f4"                             0
laddr: ADDRLP4                 "lp%a"                              0
paddr: ADDRFP4                 "lp+%a"                             0
gaddr: ADDRGP4                 "dp+%a"                             0
addr:  gaddr                   "%0"                                0
addr:  laddr                   "%0"                                0
addr:  paddr                   "%0"                                0
daddr: addr                    "%0"                                0
daddr: reg                     "%0"                                0
rc:    con                     "%0"                                0
rc:    reg                     "%0"                                0
stmt:  reg                     ""                                  0
reg:   con                     "    %c = %a\n"                     1
reg:   addr                    "    %c = & %0\n"                   2
reg:   LOADI1(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADU1(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADI2(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADU2(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADI4(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADP4(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADU4(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
reg:   LOADF4(reg)             "    %c = %0\n"                     (move(a)-1+COST_OF_REG_COPY)
stmt:  ASGNI1(daddr,rc)        "    *%0 = %1 .i1\n"                1
stmt:  ASGNU1(daddr,rc)        "    *%0 = %1 .u1\n"                1
stmt:  ASGNI2(daddr,rc)        "    *%0 = %1 .i2\n"                1
stmt:  ASGNU2(daddr,rc)        "    *%0 = %1 .u2\n"                1
stmt:  ASGNI4(daddr,rc)        "    *%0 = %1 .i4\n"                1
stmt:  ASGNU4(daddr,rc)        "    *%0 = %1 .u4\n"                1
stmt:  ASGNP4(daddr,rc)        "    *%0 = %1 .p4\n"                1
stmt:  ASGNF4(daddr,rc)        "    *%0 = %1 .f4\n"                1
reg:   CVFF4(reg)              "    push %0\n    %c = call _convertF%aToF4\n    sp += 4\n"        1
reg:   CVFI4(reg)              "    push %0\n    %c = call _convertF%aToI4\n    sp += 4\n"        1
reg:   CVIF4(reg)              "    push %0\n    %c = call _convertI%aToF4\n    sp += 4\n"        1
reg:   CVII1(reg)              "    %c = %0 .i%a\n"                0
reg:   CVII2(reg)              "    %c = %0 .i%a\n"                0
reg:   CVII4(reg)              "    %c = %0 .i%a\n"                0
reg:   CVIU1(reg)              "    %c = %0 .i%a\n"                0
reg:   CVIU2(reg)              "    %c = %0 .i%a\n"                0
reg:   CVIU4(reg)              "    %c = %0 .i%a\n"                0
reg:   CVPU4(reg)              "    %c = %0 .p%a\n"                0
reg:   CVUI1(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUI2(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUI4(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUP4(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUU1(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUU2(reg)              "    %c = %0 .u%a\n"                0
reg:   CVUU4(reg)              "    %c = %0 .u%a\n"                0
ind:   INDIRI1(daddr)          "*%0 .i1"                           0
ind:   INDIRI2(daddr)          "*%0 .i2"                           0
ind:   INDIRI4(daddr)          "*%0 .4"                            0
ind:   INDIRU1(daddr)          "*%0 .u1"                           0
ind:   INDIRU2(daddr)          "*%0 .u2"                           0
ind:   INDIRU4(daddr)          "*%0 .4"                            0
ind:   INDIRP4(daddr)          "*%0 .4"                            0
ind:   INDIRF4(daddr)          "*%0 .4"                            0
reg:   ind                     "    %c = %0\n"                     1
reg:   CVII1(ind)              "    %c = %0\n"                     1
reg:   CVII2(ind)              "    %c = %0\n"                     1
reg:   CVII4(ind)              "    %c = %0\n"                     1
reg:   CVIU1(ind)              "    %c = %0\n"                     1
reg:   CVIU2(ind)              "    %c = %0\n"                     1
reg:   CVIU4(ind)              "    %c = %0\n"                     1
reg:   CVPU4(ind)              "    %c = %0\n"                     1
reg:   CVUI1(ind)              "    %c = %0\n"                     1
reg:   CVUI2(ind)              "    %c = %0\n"                     1
reg:   CVUI4(ind)              "    %c = %0\n"                     1
reg:   CVUP4(ind)              "    %c = %0\n"                     1
reg:   CVUU1(ind)              "    %c = %0\n"                     1
reg:   CVUU2(ind)              "    %c = %0\n"                     1
reg:   CVUU4(ind)              "    %c = %0\n"                     1
reg:   BANDI4(reg,rc)          "    %c = %0 & %1\n"                1
reg:   BANDU4(reg,rc)          "    %c = %0 & %1\n"                1
reg:   BORI4(reg,rc)           "    %c = %0 | %1\n"                1
reg:   BORU4(reg,rc)           "    %c = %0 | %1\n"                1
reg:   BXORI4(reg,rc)          "    %c = %0 ^ %1\n"                1
reg:   BXORU4(reg,rc)          "    %c = %0 ^ %1\n"                1
reg:   NEGF4(reg)              "    neg.f4  %c = - %0\n"           1
reg:   NEGI4(reg)              "    neg.i4  %c = - %0\n"           1
reg:   BCOMI4(reg)             "    %c = ~ %0\n"                   1
reg:   BCOMU4(reg)             "    %c = ~ %0\n"                   1
reg:   LSHI4(reg,rc)           "    %c = $%0 << %1\n"              1
reg:   LSHU4(reg,rc)           "    %c = $%0 << %1\n"              1
reg:   RSHI4(reg,rc)           "    %c = $%0 >> %1 .i\n"           1
reg:   RSHU4(reg,rc)           "    %c = $%0 >> %1 .u\n"           1
reg:   ADDI4(reg,rc)           "    %c = %0 + %1\n"                1
reg:   ADDU4(reg,rc)           "    %c = %0 + %1\n"                1
reg:   ADDP4(reg,rc)           "    %c = %0 + %1\n"                1
reg:   ADDF4(reg,reg)          "    push %0\n    push %1\n    %c = call _addF4\n    sp += 8\n"   1
reg:   SUBI4(reg,rc)           "    %c = %0 - %1\n"                1
reg:   SUBU4(reg,rc)           "    %c = %0 - %1\n"                1
reg:   SUBP4(reg,rc)           "    %c = %0 - %1\n"                1
reg:   SUBF4(reg,reg)          "    push %0\n    push %1\n    %c = call _subF4\n    sp += 8\n"   1
reg:   DIVI4(reg,reg)          "    push %0\n    push %1\n    %c = call _divF4\n    sp += 8\n"   1
reg:   DIVU4(reg,reg)          "    push %0\n    push %1\n    %c = call _divu4\n    sp += 8\n"   1
reg:   DIVF4(reg,reg)          "    push %0\n    push %1\n    %c = call _divF4\n    sp += 8\n"   1
reg:   MULI4(reg,reg)          "    push %0\n    push %1\n    %c = call _muli4\n    sp += 8\n"   1
reg:   MULU4(reg,reg)          "    push %0\n    push %1\n    %c = call _mulu4\n    sp += 8\n"   1
reg:   MULF4(reg,reg)          "    push %0\n    push %1\n    %c = call _mulF4\n    sp += 8\n"   1
reg:   MODI4(reg,reg)          "    push %0\n    push %1\n    %c = call _modi4\n    sp += 8\n"   1
reg:   MODU4(reg,reg)          "    push %0\n    push %1\n    %c = call _modu4\n    sp += 8\n"   1
ar:    ADDRGP4                 "%a"                                0
ar:    reg                     "%0"                                1
stmt:  LABELV                  "%a:\n"                             0
stmt:  JUMPV(ar)               "    jmp %0\n"                      1
stmt:  EQI4(reg,con)           "    jmp %a if %0 == %1\n"          1
stmt:  EQI4(reg,reg)           "    jmp %a if %0 == %1\n"          1
stmt:  EQU4(reg,reg)           "    jmp %a if %0 == %1\n"          1
stmt:  GEI4(reg,reg)           "    jmp %a if %0 >= %1 .i\n"       1
stmt:  GEU4(reg,reg)           "    jmp %a if %0 >= %1 .u\n"       1
stmt:  GTI4(reg,reg)           "    jmp %a if %0 >  %1 .i\n"       1
stmt:  GTU4(reg,reg)           "    jmp %a if %0 >  %1 .u\n"       1
stmt:  LEI4(reg,reg)           "    jmp %a if %0 <= %1 .i\n"       1
stmt:  LEU4(reg,reg)           "    jmp %a if %0 <= %1 .u\n"       1
stmt:  LTI4(reg,reg)           "    jmp %a if %0 <  %1 .i\n"       1
stmt:  LTU4(reg,reg)           "    jmp %a if %0 <  %1 .u\n"       1
stmt:  NEI4(reg,reg)           "    jmp %a if %0 != %1\n"          1
stmt:  NEU4(reg,reg)           "    jmp %a if %0 != %1\n"          1
stmt:  EQF4(reg,reg)           "    jmpIfEQf4(%a, %0, %1)\n"       2
stmt:  LEF4(reg,reg)           "    jmpIfLEf4(%a, %0, %1)\n"       2
stmt:  LTF4(reg,reg)           "    jmpIfLTf4(%a, %0, %1)\n"       2
stmt:  GEF4(reg,reg)           "    jmpIfGEf4(%a, %0, %1)\n"       2
stmt:  GTF4(reg,reg)           "    jmpIfGTf4(%a, %0, %1)\n"       2
stmt:  NEF4(reg,reg)           "    jmpIfNEf4(%a, %0, %1)\n"       2
reg:   CALLF4(ar)              "    %c = call %0\n    sp += %a\n"  hasargs(a)
reg:   CALLI4(ar)              "    %c = call %0\n    sp += %a\n"  hasargs(a)
reg:   CALLP4(ar)              "    %c = call %0\n    sp += %a\n"  hasargs(a)
reg:   CALLU4(ar)              "    %c = call %0\n    sp += %a\n"  hasargs(a)
stmt:  CALLV(ar)               "    call %0\n    sp += %a\n"       hasargs(a)
reg:   CALLF4(ar)              "    %c = call %0\n"                hasNoArgs(a)
reg:   CALLI4(ar)              "    %c = call %0\n"                hasNoArgs(a)
reg:   CALLP4(ar)              "    %c = call %0\n"                hasNoArgs(a)
reg:   CALLU4(ar)              "    %c = call %0\n"                hasNoArgs(a)
stmt:  CALLV(ar)               "    call %0\n"                     hasNoArgs(a)
reg:   CALLI4(ar)              "    %c = %0()\n"                   onlyIfIntrinsic(a)
reg:   CALLP4(ar)              "    %c = %0()\n"                   onlyIfIntrinsic(a)
reg:   CALLU4(ar)              "    %c = %0()\n"                   onlyIfIntrinsic(a)
reg:   CALLF4(ar)              "    %c = %0()\n"                   onlyIfIntrinsic(a)
stmt:  CALLV(ar)               "    call %0\n"                     onlyIfIntrinsic(a)
stmt:  CALLB(ar,ar)            "    docall "                       1
arc:   reg                     "%0"                                0
arc:   con                     "%0"                                0
arc:   addr                    "& %0"                              0
stmt:  RETF4(arc)              "    result.f4 = %0\n"              1
stmt:  RETI4(arc)              "    result.i4 = %0\n"              1
stmt:  RETU4(arc)              "    result.u4 = %0\n"              1
stmt:  RETP4(arc)              "    result.u4 = %0\n"              1
stmt:  ARGF4(arc)              "    push %0\n"                     1
stmt:  ARGI4(arc)              "    push %0\n"                     1
stmt:  ARGP4(arc)              "    push %0\n"                     1
stmt:  ARGU4(arc)              "    push %0\n"                     1
stmt:  ARGB(INDIRB(daddr))     "    push *%0 .%a align=%b\n"       1
stmt:  ARGB(INDIRB(reg))       "    <ARGB>\n"                      10
stmt:  ASGNB(reg,INDIRB(reg))  "    copy *%0 = *%1 .%a align=%b\n" 1
%%
/*


*/
static void progend(void){}
static void progbeg(int argc, char *argv[]) {
        int i;

        {
                union {
                        char c;
                        int i;
                } u;
                u.i = 0;
                u.c = 1;
                swap = ((int)(u.i == 1)) != IR->little_endian;
        }
        print("; Split16 Assembly file, generated by LCC 4.2 \n");
        pic = !IR->little_endian;
        parseflags(argc, argv);
        for (i = 0; i < argc; i++)
                if (strncmp(argv[i], "-G", 2) == 0)
                        gnum = atoi(argv[i] + 2);
                else if (strcmp(argv[i], "-pic=1") == 0
                ||       strcmp(argv[i], "-pic=0") == 0)
                        pic = argv[i][5]-'0';
        for (i = 0; i < 32; i++)
                reg64[i] = mkreg("drg%d", 31-i, 1, REG8);
        for (i = 0; i < 32; i++)
                reg32[i]  = mkreg("r%d", 31-i, 1, REG4);
        d6 = mkreg("6", 6, 3, REG4);
        reg64wildcard = mkwildcard(reg64);
        reg32wildcard = mkwildcard(reg32);
        tmask[REG4] = INTTMP; tmask[REG8] = FLTTMP;
        vmask[REG4] = INTVAR; vmask[REG8] = FLTVAR;
        blkreg = mkreg("8", 8, 7, IREG);
}
static Symbol rmap(int opk) {
        return reg32wildcard;
        /*
        if (optype(opk) == B) {
                return reg32wildcard;
        }
        switch (opsize(opk))
        {
                case 1: case 2: case 4:
                        return reg32wildcard;
                case 8:
                        return reg64wildcard;
                default:
                        assert(0);
        }
        */
}
static void target(Node p) {

}
static void clobber(Node p) {}
static void emit2(Node p) {}
static void doarg(Node p) {
        static int argno;
        int align;

        if (argoffset == 0)
                argno = 0;
        p->x.argno = argno++;
        align = p->syms[1]->u.c.v.i < 4 ? 4 : p->syms[1]->u.c.v.i;
        p->syms[2] = intconst(mkactual(align,
                p->syms[0]->u.c.v.i));
}
static void local(Symbol p) {
        if (askregvar(p, rmap(ttob(p->type))) == 0)
                 mkauto(p);
}

#define SIZEOFADAMADDRESS 3
static void function(Symbol f, Symbol caller[], Symbol callee[], int ncalls) {
        int i, reg4Count, reg8Count;

        print("%s:\n", f->x.name);
        usedmask[0] = usedmask[1] = 0;
        freemask[0] = freemask[1] = ~(unsigned)0;
        // save space for the return execution address
        // and the return value address
        offset = roundup(SIZEOFADAMADDRESS*2, 4);
        for (i = 0; callee[i]; i++) {
                Symbol p = callee[i];
                Symbol q = caller[i];
                assert(q);
                p->x.offset = q->x.offset = offset;
                /*p->x.name = q->x.name = stringf("%d<id %d>", p->x.offset, i);*/
                p->x.name = q->x.name = stringf("%d", p->x.offset);
                p->sclass = q->sclass = AUTO;
                offset += roundup(q->type->size, 4);
        }
        assert(caller[i] == 0);
        offset = maxoffset = 0;
        gencode(caller, callee);
        framesize = roundup(maxoffset, 4);
        reg4Count = bitcount(usedmask[REG4]);
        reg8Count = bitcount(usedmask[REG8]);
        assert(reg8Count==0);
        if(reg4Count || framesize){
            printf("    enter regCount=%d, localBytes=%d\n", reg4Count, framesize);
        }
        emitcode();
        if(reg4Count || framesize){
          printf("    exitAndReturn\n");
        } else {
          printf("    Return\n");
        }
        print("\n");
}
static void defconst(int suffix, int size, Value v) {
        if (suffix == F && size == 4) {
                float f = v.d;
                print(".def4 0x%x\n", *(unsigned *)&f);
        }
        else if (suffix == F && size == 8) {
                double d = v.d;
                unsigned *p = (unsigned *)&d;
                print(".def4 0x%x\n..def4 0x%x\n", p[swap], p[!swap]);
        }
        else if (suffix == P)
                print(".def4 0x%x\n", (unsigned)v.p);
        else if (size == 1)
                print(".def1 0x%x\n", (unsigned)((unsigned char)(suffix == I ? v.i : v.u)));
        else if (size == 2)
                print(".def2 0x%x\n", (unsigned)((unsigned short)(suffix == I ? v.i : v.u)));
        else if (size == 4)
                print(".def4 0x%x\n", (unsigned)(suffix == I ? v.i : v.u));
}
static void defaddress(Symbol p) {
        print(".def4 %s\n", p->x.name);
}
static void defstring(int n, char *str) {
        char *s;

        for (s = str; s < str + n; s++)
                print(".def1 %d\n", (*s)&0377);
}
static void export(Symbol p) {
        print(".globl %s\n", p->x.name);
}
static void import(Symbol p) {
        if (!isfunc(p->type))
                print(".extern %s\n", p->name);
}
static void defsymbol(Symbol p) {
        if (p->scope >= LOCAL && p->sclass == STATIC)
                p->x.name = stringf("_L%d", genlabel(1));
        else if (p->generated)
                p->x.name = stringf("_L%s", p->name);
        else if(p->scope == CONSTANTS && isint(p->type))
        {
                p->x.name = stringf("%d", p->u.c.v.i);
        } 
        else {
                assert(p->scope != CONSTANTS || isptr(p->type) || isfloat(p->type)),
                        p->x.name = p->name;
        }
}
static void address(Symbol q, Symbol p, long n) {
        if (p->scope == GLOBAL
        || p->sclass == STATIC || p->sclass == EXTERN)
                q->x.name = stringf("%s%s%D", p->x.name,
                        n >= 0 ? "+" : "", n);
        else {
                assert(n <= INT_MAX && n >= INT_MIN);
                q->x.offset = p->x.offset + n;
                q->x.name = stringd(q->x.offset);
        }
}
static void global(Symbol p) {
        print(".align %d\n", p->type->align);
        print("%s:\n", p->x.name);
        if (p->u.seg == BSS) {
                print( ".reserve %d\n", p->type->size);
        }
}
static void segment(int n) {
        cseg = n;
        switch (n) {
        case CODE: print("\n.text\n");  break;
        case LIT:  print("\n.lit\n"); break;
        case BSS: print("\n.bss\n");  break;
        case DATA:  print("\n.data\n"); break;
        }
}
static void space(int n) {
        if (cseg != BSS)
                print(".reserve %d\n", n);
}

static void blkloop(int dreg, int doff, int sreg, int soff, int size, int tmps[]) {
        /*int lab = genlabel(1);

        print("addu $%d,$%d,%d\n", sreg, sreg, size&~7);
        print("addu $%d,$%d,%d\n", tmps[2], dreg, size&~7);
        blkcopy(tmps[2], doff, sreg, soff, size&7, tmps);
        print("L_%d:\n", lab);
        print("addu $%d,$%d,%d\n", sreg, sreg, -8);
        print("addu $%d,$%d,%d\n", tmps[2], tmps[2], -8);
        blkcopy(tmps[2], doff, sreg, soff, 8, tmps);
        print("bltu $%d,$%d,L_%d\n", dreg, tmps[2], lab);*/
}
static void blkfetch(int size, int off, int reg, int tmp) {
     /*   assert(size == 1 || size == 2 || size == 4);
        if (size == 1)
                print("lbu $%d,%d($%d)\n",  tmp, off, reg);
        else if (salign >= size && size == 2)
                print("lhu $%d,%d($%d)\n",  tmp, off, reg);
        else if (salign >= size)
                print("lw $%d,%d($%d)\n",   tmp, off, reg);
        else if (size == 2)
                print("ulhu $%d,%d($%d)\n", tmp, off, reg);
        else
                print("ulw $%d,%d($%d)\n",  tmp, off, reg);*/
}
static void blkstore(int size, int off, int reg, int tmp) {
  /*      if (size == 1)
                print("sb $%d,%d($%d)\n",  tmp, off, reg);
        else if (dalign >= size && size == 2)
                print("sh $%d,%d($%d)\n",  tmp, off, reg);
        else if (dalign >= size)
                print("sw $%d,%d($%d)\n",  tmp, off, reg);
        else if (size == 2)
                print("ush $%d,%d($%d)\n", tmp, off, reg);
        else
                print("usw $%d,%d($%d)\n", tmp, off, reg);*/
}
static void stabinit(char *, int, char *[]);
static void stabline(Coordinate *);
static void stabsym(Symbol);

static char *currentfile;

static int bitcount(unsigned mask) {
        unsigned i, n = 0;

        for (i = 1; i; i <<= 1)
                if (mask&i)
                        n++;
        return n;
}


/* stabinit - initialize stab output */
static void stabinit(char *file, int argc, char *argv[]) {
        if (file) {
                print(".file 2,\"%s\"\n", file);
                currentfile = file;
        }
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(Coordinate *cp) {
        if (cp->file && cp->file != currentfile) {
                print(".file 2,\"%s\"\n", cp->file);
                currentfile = cp->file;
        }
        print(".loc 2,%d\n", cp->y);
}

/* stabsym - output a stab entry for symbol p */
static void stabsym(Symbol p) {
        if (p == cfunc && IR->stabline)
                (*IR->stabline)(&p->src);
}
Interface adamStdIR = {
        1, 1, 0,  /* char */
        2, 2, 0,  /* short */
        4, 4, 0,  /* int */
        4, 4, 0,  /* long */
        4, 4, 0,  /* long long */
        4, 4, 0,  /* float */
        4, 4, 0,  /* double */
        4, 4, 1,  /* long double */
        4, 4, 0,  /* T * */
        0, 1, 0,  /* struct */
        1,      /* big_endian */
        0,  /* mulops_calls */
        0,  /* wants_callb */
        1,  /* wants_argb */
        0,  /* left_to_right */
        0,  /* wants_dag */
        0,  /* unsigned_char */
        address,
        blockbeg,
        blockend,
        defaddress,
        defconst,
        defstring,
        defsymbol,
        emit,
        export,
        function,
        gen,
        global,
        import,
        local,
        progbeg,
        progend,
        segment,
        space,
        0, 0, 0, stabinit, stabline, stabsym, 0,
        {
                4,      /* max_unaligned_load */
                rmap,
                blkfetch, blkstore, blkloop,
                _label,
                _rule,
                _nts,
                _kids,
                _string,
                _templates,
                _isinstruction,
                _ntname,
                emit2,
                doarg,
                target,
                clobber,

        }
};

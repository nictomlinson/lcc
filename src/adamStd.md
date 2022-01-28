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

#define readsreg(p) \
        (generic((p)->op)==INDIR && (p)->kids[0]->op==VREG+P)
#define setsrc(d) ((d) && (d)->x.regnode && \
        (d)->x.regnode->set == src->x.regnode->set && \
        (d)->x.regnode->mask&src->x.regnode->mask)

#define relink(a, b) ((b)->x.prev = (a), (a)->x.next = (b))

#include "c.h"
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
%term CNSTF8=8209
%term CNSTF16=16401
%term CNSTI1=1045
%term CNSTI2=2069
%term CNSTI4=4117
%term CNSTI8=8213
%term CNSTP4=4119
%term CNSTP8=8215
%term CNSTU1=1046
%term CNSTU2=2070
%term CNSTU4=4118
%term CNSTU8=8214
 
%term ARGB=41
%term ARGF4=4129
%term ARGF8=8225
%term ARGF16=16417
%term ARGI4=4133
%term ARGI8=8229
%term ARGP4=4135
%term ARGP8=8231
%term ARGU4=4134
%term ARGU8=8230

%term ASGNB=57
%term ASGNF4=4145
%term ASGNF8=8241
%term ASGNF16=16433
%term ASGNI1=1077
%term ASGNI2=2101
%term ASGNI4=4149
%term ASGNI8=8245
%term ASGNP4=4151
%term ASGNP8=8247
%term ASGNU1=1078
%term ASGNU2=2102
%term ASGNU4=4150
%term ASGNU8=8246

%term INDIRB=73
%term INDIRF4=4161
%term INDIRF8=8257
%term INDIRF16=16449
%term INDIRI1=1093
%term INDIRI2=2117
%term INDIRI4=4165
%term INDIRI8=8261
%term INDIRP4=4167
%term INDIRP8=8263
%term INDIRU1=1094
%term INDIRU2=2118
%term INDIRU4=4166
%term INDIRU8=8262

%term CVFF4=4209
%term CVFF8=8305
%term CVFF16=16497
%term CVFI4=4213
%term CVFI8=8309

%term CVIF4=4225
%term CVIF8=8321
%term CVIF16=16513
%term CVII1=1157
%term CVII2=2181
%term CVII4=4229
%term CVII8=8325
%term CVIU1=1158
%term CVIU2=2182
%term CVIU4=4230
%term CVIU8=8326

%term CVPP4=4247
%term CVPP8=8343
%term CVPP16=16535
%term CVPU4=4246
%term CVPU8=8342

%term CVUI1=1205
%term CVUI2=2229
%term CVUI4=4277
%term CVUI8=8373
%term CVUP4=4279
%term CVUP8=8375
%term CVUP16=16567
%term CVUU1=1206
%term CVUU2=2230
%term CVUU4=4278
%term CVUU8=8374

%term NEGF4=4289
%term NEGF8=8385
%term NEGF16=16577
%term NEGI4=4293
%term NEGI8=8389

%term CALLB=217
%term CALLF4=4305
%term CALLF8=8401
%term CALLF16=16593
%term CALLI4=4309
%term CALLI8=8405
%term CALLP4=4311
%term CALLP8=8407
%term CALLU4=4310
%term CALLU8=8406
%term CALLV=216

%term RETF4=4337
%term RETF8=8433
%term RETF16=16625
%term RETI4=4341
%term RETI8=8437
%term RETP4=4343
%term RETP8=8439
%term RETU4=4342
%term RETU8=8438
%term RETV=248

%term ADDRGP4=4359
%term ADDRGP8=8455

%term ADDRFP4=4375
%term ADDRFP8=8471

%term ADDRLP4=4391

%term ADDF4=4401
%term ADDF8=8497
%term ADDF16=16689
%term ADDI4=4405
%term ADDI8=8501
%term ADDP4=4407
%term ADDP8=8503
%term ADDU4=4406
%term ADDU8=8502

%term SUBF4=4417
%term SUBF8=8513
%term SUBF16=16705
%term SUBI4=4421
%term SUBI8=8517
%term SUBP4=4423
%term SUBP8=8519
%term SUBU4=4422
%term SUBU8=8518

%term LSHI4=4437
%term LSHI8=8533
%term LSHU4=4438
%term LSHU8=8534

%term MODI4=4453
%term MODI8=8549
%term MODU4=4454
%term MODU8=8550

%term RSHI4=4469
%term RSHI8=8565
%term RSHU4=4470
%term RSHU8=8566

%term BANDI4=4485
%term BANDI8=8581
%term BANDU4=4486
%term BANDU8=8582

%term BCOMI4=4501
%term BCOMI8=8597
%term BCOMU4=4502
%term BCOMU8=8598

%term BORI4=4517
%term BORI8=8613
%term BORU4=4518
%term BORU8=8614

%term BXORI4=4533
%term BXORI8=8629
%term BXORU4=4534
%term BXORU8=8630

%term DIVF4=4545
%term DIVF8=8641
%term DIVF16=16833
%term DIVI4=4549
%term DIVI8=8645
%term DIVU4=4550
%term DIVU8=8646

%term MULF4=4561
%term MULF8=8657
%term MULF16=16849
%term MULI4=4565
%term MULI8=8661
%term MULU4=4566
%term MULU8=8662

%term EQF4=4577
%term EQF8=8673
%term EQF16=16865
%term EQI2=2533
%term EQI4=4581
%term EQI8=8677
%term EQU4=4582
%term EQU8=8678

%term GEF4=4593
%term GEF8=8689
%term GEI4=4597
%term GEI8=8693
%term GEI16=16885
%term GEU4=4598
%term GEU8=8694

%term GTF4=4609
%term GTF8=8705
%term GTF16=16897
%term GTI4=4613
%term GTI8=8709
%term GTU4=4614
%term GTU8=8710

%term LEF4=4625
%term LEF8=8721
%term LEF16=16913
%term LEI4=4629
%term LEI8=8725
%term LEU4=4630
%term LEU8=8726

%term LTF4=4641
%term LTF8=8737
%term LTF16=16929
%term LTI4=4645
%term LTI8=8741
%term LTU4=4646
%term LTU8=8742

%term NEF4=4657
%term NEF8=8753
%term NEF16=16945
%term NEI4=4661
%term NEI8=8757
%term NEU4=4662
%term NEU8=8758

%term JUMPV=584

%term LABELV=600

%term LOADB=233
%term LOADF4=4321
%term LOADF8=8417
%term LOADF16=16609
%term LOADI1=1253
%term LOADI2=2277
%term LOADI4=4325
%term LOADI8=8421
%term LOADP4=4327
%term LOADP8=8423
%term LOADU1=1254
%term LOADU2=2278
%term LOADU4=4326
%term LOADU8=8422

%term VREGP=711
%%
reg:  INDIRI1(VREGP)     "# read register\n"
reg:  INDIRU1(VREGP)     "# read register\n"

reg:  INDIRI2(VREGP)     "# read register\n"
reg:  INDIRU2(VREGP)     "# read register\n"

reg:  INDIRF4(VREGP)     "# read register\n"
reg:  INDIRI4(VREGP)     "# read register\n"
reg:  INDIRP4(VREGP)     "# read register\n"
reg:  INDIRU4(VREGP)     "# read register\n"

reg:  INDIRF8(VREGP)     "# read register\n"
reg:  INDIRI8(VREGP)     "# read register\n"
reg:  INDIRP8(VREGP)     "# read register\n"
reg:  INDIRU8(VREGP)     "# read register\n"

stmt: ASGNI1(VREGP,reg)  "# write register\n"
stmt: ASGNU1(VREGP,reg)  "# write register\n"

stmt: ASGNI2(VREGP,reg)  "# write register\n"
stmt: ASGNU2(VREGP,reg)  "# write register\n"

stmt: ASGNF4(VREGP,reg)  "# write register\n"
stmt: ASGNI4(VREGP,reg)  "# write register\n"
stmt: ASGNP4(VREGP,reg)  "# write register\n"
stmt: ASGNU4(VREGP,reg)  "# write register\n"

stmt: ASGNF8(VREGP,reg)  "# write register\n"
stmt: ASGNI8(VREGP,reg)  "# write register\n"
stmt: ASGNP8(VREGP,reg)  "# write register\n"
stmt: ASGNU8(VREGP,reg)  "# write register\n"

con4: CNSTI1  "%a"
con4: CNSTU1  "%a"
con4: CNSTI2  "%a"
con4: CNSTU2  "%a"
con4: CNSTI4  "%a"
con4: CNSTU4  "%a"
con4: CNSTP4  "%a"
conf4: CNSTF4  "%a"
con8: CNSTI8  "%a"
con8: CNSTU8  "%a"
conf8: CNSTF8  "%a"

con: con4 "%0"
con: con8 "%0"
con: conf4 "%0"
con: conf8 "%0"

codeaddr: ADDRGP4         "%a"
dataaddr: ADDRGP4         "global[%a]"
dataaddr: ADDRFP4         "arg[%a]"
dataaddr: ADDRLP4         "local[%a]"

addr: dataaddr        "%0"


addrRegCon: reg "%0"
addrRegCon: con "%0"
addrRegCon: addr "%0"


rc:  con            "%0"
rc:  reg            "%0"

rc5: CNSTI4         "%a"                range(a,0,31)
rc5: reg            "%0"


ar:   ADDRGP4     "%a"
ar:   reg         "%0"
ar:   CNSTP4      "CNSTP4:%a"   range(a, 0, 0x0fffffff)


stmt: reg  ""



reg: CNSTI1  "const.1 %c = %a\n"
reg: CNSTU1  "const.1 %c = %a\n"
reg: CNSTI2  "const.2 %c = %a\n"
reg: CNSTU2  "const.2 %c = %a\n"
reg: CNSTI4  "const.4 %c = %a\n"
reg: CNSTU4  "const.4 %c = %a\n"
reg: CNSTP4  "const.4 %c = %a\n"
reg: CNSTF4  "const.f4 %c = %a\n"
reg: CNSTI8  "const.8 %c = %a\n"
reg: CNSTU8  "const.8 %c = %a\n"
reg: CNSTF8  "const.f8 %c = %a\n"


reg: dataaddr                 "loadAddrOf %c = %0\n"      1
reg: ADDP4(dataaddr, con)     "loadAddrOf %c = %0 + %1\n" 1

reg: LOADI1(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADU1(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADI2(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADU2(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADI4(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADP4(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADU4(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADF4(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_REG_COPY)
reg: LOADF8(reg)        "copy %c = %0\n"  (move(a)-1+COST_OF_DRG_COPY)


stmt: ASGNI1(dataaddr,reg)  "stor.1 %0 = %1\n"  1
stmt: ASGNU1(dataaddr,reg)  "stor.1 %0 = %1\n"  1
stmt: ASGNI2(dataaddr,reg)  "stor.2 %0 = %1\n"  1
stmt: ASGNU2(dataaddr,reg)  "stor.2 %0 = %1\n"  1
stmt: ASGNI4(dataaddr,reg)  "stor.4 %0 = %1\n"  1
stmt: ASGNU4(dataaddr,reg)  "stor.4 %0 = %1\n"  1
stmt: ASGNP4(dataaddr,reg)  "stor.4 %0 = %1\n"  1
stmt: ASGNF4(dataaddr,reg)  "stor.4 %0 = %1\n"  1
stmt: ASGNF8(dataaddr,reg)  "stor.8 %0 = %1\n"  1


stmt: ASGNI1(reg,reg)  "stor.1 *%0 = %1\n"  1
stmt: ASGNU1(reg,reg)  "stor.1 *%0 = %1\n"  1
stmt: ASGNI2(reg,reg)  "stor.2 *%0 = %1\n"  1
stmt: ASGNU2(reg,reg)  "stor.2 *%0 = %1\n"  1
stmt: ASGNI4(reg,reg)  "stor.4 *%0 = %1\n"  1
stmt: ASGNU4(reg,reg)  "stor.4 *%0 = %1\n"  1
stmt: ASGNP4(reg,reg)  "stor.4 *%0 = %1\n"  1
stmt: ASGNF4(reg,reg)  "stor.4 *%0 = %1\n"  1
stmt: ASGNF8(reg,reg)  "stor.8 *%0 = %1\n"  1


reg:  INDIRI1(dataaddr)     "load.i1 %c = %0\n"  1
reg:  INDIRU1(dataaddr)     "load.u2 %c = %0\n"  1
reg:  INDIRI2(dataaddr)     "load.i2 %c = %0\n"  1
reg:  INDIRU2(dataaddr)     "load.u2 %c = %0\n"  1
reg:  INDIRI4(dataaddr)     "load.i4 %c = %0\n"  10
reg:  INDIRU4(dataaddr)     "load.u4 %c = %0\n"  1
reg:  INDIRP4(dataaddr)     "load.p4 %c = %0\n"  1
reg:  INDIRF4(dataaddr)     "load.f4 %c = %0\n"  10
reg:  INDIRF8(dataaddr)     "load.f8 %c = %0\n"  1

reg:  INDIRI1(reg)     "load.i1 %c =* %0\n"  1
reg:  INDIRU1(reg)     "load.u2 %c =* %0\n"  1
reg:  INDIRI2(reg)     "load.i2 %c =* %0\n"  1
reg:  INDIRU2(reg)     "load.u2 %c =* %0\n"  1
reg:  INDIRI4(reg)     "load.i4 %c =* %0\n"  10
reg:  INDIRU4(reg)     "load.u4 %c =* %0\n"  1
reg:  INDIRP4(reg)     "load.p4 %c =* %0\n"  1
reg:  INDIRF4(reg)     "load.f4 %c =* %0\n"  10
reg:  INDIRF8(reg)     "load.f8 %c =* %0\n"  1



reg:  CVII4(INDIRI1(dataaddr))     "<X> load.i1 %c = %0\n"  
reg:  CVUI4(INDIRU1(dataaddr))     "<X> load.u1 %c = %0\n"  
reg:  CVII4(INDIRI2(dataaddr))     "<X> load.i2 %c = %0\n" 5
reg:  CVUU4(INDIRU2(dataaddr))     "<X> load.u2 %c = %0\n"  

reg: CVII4(reg)  "<X> sll $%c,$%0,8*(4-%a); sra $%c,$%c,8*(4-%a)\n" 10
reg: CVUI4(reg)  "<X> and $%c,$%0,(1<<(8*%a))-1\n"  
reg: CVUU4(reg)  "<X> and $%c,$%0,(1<<(8*%a))-1\n"  
reg: CVFF4(reg)  "<X> cvt.s.d $f%c,$f%0\n"  
reg: CVFF8(reg)  "<X> cvt.d.s $f%c,$f%0\n"  
reg: CVIF4(reg)  "<X> mtc1 $%0,$f%c; cvt.s.w $f%c,$f%c\n"  
reg: CVIF8(reg)  "<X> mtc1 $%0,$f%c; cvt.d.w $f%c,$f%c\n"  
reg: CVFI4(reg)  "<X> trunc.w.s $f2,$f%0,$%c; mfc1 $%c,$f2\n"  (a->syms[0]->u.c.v.i==4?2:LBURG_MAX)
reg: CVFI4(reg)  "<X> trunc.w.d $f2,$f%0,$%c; mfc1 $%c,$f2\n"  (a->syms[0]->u.c.v.i==8?2:LBURG_MAX)




reg: BANDI4(reg,rc)  "and.i4  %c = %0 & %1\n"   1
reg: BANDU4(reg,rc)  "and.u4  %c = %0 & %1\n"   1
reg: BORI4(reg,rc)   "or.i4   %c = %0 | %1\n"    1
reg: BORU4(reg,rc)   "or.u4   %c = %0 | %1\n"    1
reg: BXORI4(reg,rc)  "xor.i4  %c = %0 ^ %1\n"   1
reg: BXORU4(reg,rc)  "xor.u4  %c = %0 ^ %1\n"   1
reg: NEGF4(reg)      "neg.f4  %c = NEG? %0\n"       1
reg: NEGF8(reg)      "neg.f8  %c = NEG? %0\n"       1
reg: NEGI4(reg)      "neg.i4  %c = NEG? %0\n"  1
reg: BCOMI4(reg)     "not.i4  %c = BCOM? %0\n"   1
reg: BCOMU4(reg)     "not.u4  %c = BCOM? %0\n"   1
reg: LSHI4(reg,rc5)  "lsh.i4  %c = $%0 << %1\n"  1
reg: LSHU4(reg,rc5)  "lsh.u4  %c = $%0 << %1\n"  1
reg: RSHI4(reg,rc5)  "rsh.i4  %c = $%0 >> %1\n"  1
reg: RSHU4(reg,rc5)  "rsh.u4  %c = $%0 >> %1\n"  1
reg: ADDI4(reg,rc)   "add.i4  %c = %0 + %1\n"  1
reg: ADDU4(reg,rc)   "add.u4  %c = %0 + %1\n"  1
reg: ADDP4(reg,rc)   "add.p4  %c = %0 + %1\n"  1
reg: ADDF4(reg,reg)  "add.f4  %c = %0 + %1\n"  1
reg: ADDF8(reg,reg)  "add.f8  %c = %0 + %1\n"  1
reg: SUBI4(reg,rc)   "sub.i4  %c = %0 - %1\n"  1
reg: SUBU4(reg,rc)   "sub.u4  %c = %0 - %1\n"  1
reg: SUBP4(reg,rc)   "sub.p4  %c = %0 - %1\n"  1
reg: SUBF4(reg,reg)  "sub.f4  %c = %0 - %1\n"  1
reg: SUBF8(reg,reg)  "sub.f8  %c = %0 - %1\n"  1
reg: DIVI4(reg,reg)  "div.i4  %c = %0 / %1\n"   1
reg: DIVU4(reg,reg)  "div.u4  %c = %0 / %1\n"  1
reg: DIVF4(reg,reg)  "div.f4  %c = %0 / %1\n"  1
reg: DIVF8(reg,reg)  "div.f8  %c = %0 / %1\n"  1
reg: MULI4(reg,reg)  "mul.i4  %c = %0 * %1\n"   1
reg: MULU4(reg,reg)  "mul.u4  %c = %0 * %1\n"   1
reg: MULF4(reg,reg)  "mul.f4  %c = %0 * %1\n"  1
reg: MULF8(reg,reg)  "mul.f8  %c = %0 * %1\n"  1
reg: MODI4(reg,reg)  "mod.i4  %c = %0 % %1\n"   1
reg: MODU4(reg,reg)  "mod.u4  %c = %0 % %1\n"  1




stmt: LABELV         "%a:\n"

stmt: JUMPV(reg)     "jmp* %0\n" 1
stmt: JUMPV(codeaddr)    "jmp %0\n" 1

stmt: EQI4(reg,con)  "jmp ??? %a if.4 %0 == %1\n"   1
stmt: EQI2(reg,con)  "jmp ??? %a if.2 %0 == %1\n"   1

stmt: EQI4(reg,reg)  "jmp ??? %a if    %0 == %1\n"   5
stmt: EQU4(reg,reg)  "jmp ??? %a if.u4 %0 == %1\n"   1
stmt: GEI4(reg,reg)  "jmp ??? %a if    %0 >= %1\n"   1
stmt: GEU4(reg,reg)  "jmp ??? %a if.u4 %0 >= %1\n"  1
stmt: GTI4(reg,reg)  "jmp ??? %a if    %0 >  %1\n"   1
stmt: GTU4(reg,reg)  "jmp ??? %a if.u4 %0 >  %1\n"  1
stmt: LEI4(reg,reg)  "jmp ??? %a if    %0 <= %1\n"   1
stmt: LEU4(reg,reg)  "jmp ??? %a if.u4 %0 <= %1\n"  1
stmt: LTI4(reg,reg)  "jmp ??? %a if    %0 <  %1\n"   1
stmt: LTU4(reg,reg)  "jmp ??? %a if.u4 %0 <  %1\n"  1
stmt: NEI4(reg,reg)  "jmp ??? %a if    %0 != %1\n"   1
stmt: NEU4(reg,reg)  "jmp ??? %a if.u4 %0 != %1\n"   1

stmt: EQF4(reg,reg)  "BRANCH_IF_EQ_F4(%a, %0, %1)\n"  2
stmt: EQF8(reg,reg)  "BRANCH_IF_EQ_F8(%a, %0, %1)\n"  2
stmt: LEF4(reg,reg)  "BRANCH_IF_LE_F4(%a, %0, %1)\n"  2
stmt: LEF8(reg,reg)  "BRANCH_IF_LE_F8(%a, %0, %1)\n"  2
stmt: LTF4(reg,reg)  "BRANCH_IF_LT_F4(%a, %0, %1)\n"  2
stmt: LTF8(reg,reg)  "BRANCH_IF_LT_F8(%a, %0, %1)\n"  2
stmt: GEF4(reg,reg)  "BRANCH_IF_GE_F4(%a, %0, %1)\n"  2
stmt: GEF8(reg,reg)  "BRANCH_IF_GE_F8(%a, %0, %1)\n"  2
stmt: GTF4(reg,reg)  "BRANCH_IF_GT_F4(%a, %0, %1)\n"  2
stmt: GTF8(reg,reg)  "BRANCH_IF_GT_F8(%a, %0, %1)\n"  2
stmt: NEF4(reg,reg)  "BRANCH_IF_NE_F4(%a, %0, %1)\n"  2
stmt: NEF8(reg,reg)  "BRANCH_IF_NE_F8(%a, %0, %1)\n"  2


reg:  CALLF4(ar)  "f4.call  %c = %0()\nbn.pop  %a\n"  1
reg:  CALLF8(ar)  "f8.call  %c = %0()\nbn.pop  %a\n"  1
reg:  CALLI4(ar)  "i4.call  %c = %0()\nbn.pop  %a<why 4 too big but ok if left_to_right>\n"  1
reg:  CALLP4(ar)  "p4.call  %c = %0()\nbn.pop  %a\n"  1
reg:  CALLU4(ar)  "u4.call  %c = %0()\nbn.pop  %a\n"  1
stmt: CALLV(ar)   "vo.call       %0()\nbn.pop  %a\n"  1


stmt: RETF4(addrRegCon)  "exitReturning.4 %0\n"  1
stmt: RETF8(addrRegCon)  "exitReturning.8 %0\n"  1
stmt: RETI4(addrRegCon)  "exitReturning.4 %0\n"  1
stmt: RETU4(addrRegCon)  "exitReturning.4 %0\n"  1
stmt: RETP4(addrRegCon)  "exitReturning.4 %0\n"  1
stmt: RETV(addrRegCon)   "exitReturning.0 %0\n"  1

stmt: ARGF4(addrRegCon)  "push.4 %0\n"  1
stmt: ARGF8(addrRegCon)  "push.8 %0\n"  1
stmt: ARGI4(addrRegCon)  "push.4 %0\n"  1
stmt: ARGP4(addrRegCon)  "push.4 %0\n"  1
stmt: ARGU4(addrRegCon)  "push.4 %0\n"  1
 

stmt: ARGB(INDIRB(dataaddr)) "pushBytes align(.%b)\n\t\tbyteCount(%a)\n\t\tsource(%0)\n\t\texpectingZero(%c)\n"
stmt: ARGB(INDIRB(reg))       "<ARGB>\n"      10

stmt: ASGNB(reg,INDIRB(reg))  "# asgnb %0 %1\n"  1
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
        print("; Adam Standard Assembly file, generated by LCC 4.2 \n");
        pic = !IR->little_endian;
        parseflags(argc, argv);
        for (i = 0; i < argc; i++)
                if (strncmp(argv[i], "-G", 2) == 0)
                        gnum = atoi(argv[i] + 2);
                else if (strcmp(argv[i], "-pic=1") == 0
                ||       strcmp(argv[i], "-pic=0") == 0)
                        pic = argv[i][5]-'0';
        for (i = 0; i < 32; i++)
                reg64[i] = mkreg("drg[%d]", 31-i, 1, REG8);
        for (i = 0; i < 32; i++)
                reg32[i]  = mkreg("reg[%d]", 31-i, 1, REG4);
        d6 = mkreg("6", 6, 3, REG4);
        reg64wildcard = mkwildcard(reg64);
        reg32wildcard = mkwildcard(reg32);
        tmask[REG4] = INTTMP; tmask[REG8] = FLTTMP;
        vmask[REG4] = INTVAR; vmask[REG8] = FLTVAR;
        blkreg = mkreg("8", 8, 7, IREG);
}
static Symbol rmap(int opk) {
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
}
static void target(Node p) {}
static void clobber(Node p) {}
static void emit2(Node p) {
        int dst, n, src, sz, ty;
        static int ty0;
        Symbol q;

        switch (specific(p->op)) {
        case ASGN+B:
                dalign = salign = p->syms[1]->u.c.v.i;
                blkcopy(getregnum(p->x.kids[0]), 0,
                        getregnum(p->x.kids[1]), 0,
                        p->syms[0]->u.c.v.i, tmpregs);
                break;
        case ARG+B:
                dalign = 4;
                salign = p->syms[1]->u.c.v.i;
                blkcopy(29, p->syms[2]->u.c.v.i,
                        getregnum(p->x.kids[0]), 0,
                        p->syms[0]->u.c.v.i, tmpregs);
                n   = p->syms[2]->u.c.v.i + p->syms[0]->u.c.v.i;
                dst = p->syms[2]->u.c.v.i;
                for ( ; dst <= 12 && dst < n; dst += 4)
                        print("lw $%d,%d($sp)\n", (dst/4)+4, dst);
                break;
/*        default:
           print("...unknown emit2 request<%s>\n", opname(p->op)); */
        }
}
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
        printf("enter regCount=%d, drgCount=%d, localBytes=%d\n", reg4Count, reg8Count,  framesize);
        emitcode();
        print("unreachable\n");
        print("\n");
}
static void defconst(int suffix, int size, Value v) {
        if (suffix == F && size == 4) {
                float f = v.d;
                print(".word 0x%x\n", *(unsigned *)&f);
        }
        else if (suffix == F && size == 8) {
                double d = v.d;
                unsigned *p = (unsigned *)&d;
                print(".word 0x%x\n.word 0x%x\n", p[swap], p[!swap]);
        }
        else if (suffix == P)
                print(".word 0x%x\n", (unsigned)v.p);
        else if (size == 1)
                print(".byte 0x%x\n", (unsigned)((unsigned char)(suffix == I ? v.i : v.u)));
        else if (size == 2)
                print(".half 0x%x\n", (unsigned)((unsigned short)(suffix == I ? v.i : v.u)));
        else if (size == 4)
                print(".word 0x%x\n", (unsigned)(suffix == I ? v.i : v.u));
}
static void defaddress(Symbol p) {
        if (pic && p->scope == LABELS)
                print(".gpword %s\n", p->x.name);
        else
                print(".word %s\n", p->x.name);
}
static void defstring(int n, char *str) {
        char *s;

        for (s = str; s < str + n; s++)
                print(".byte %d\n", (*s)&0377);
}
static void export(Symbol p) {
        print(".globl %s\n", p->x.name);
}
static void import(Symbol p) {
        if (!isfunc(p->type))
                print(".extern %s %d\n", p->name, p->type->size);
}
static void defsymbol(Symbol p) {
        if (p->scope >= LOCAL && p->sclass == STATIC)
                p->x.name = stringf("L.%d", genlabel(1));
        else if (p->generated)
                p->x.name = stringf("L.%s", p->name);
        else if(p->scope == CONSTANTS && isint(p->type))
        {
                p->x.name = stringf("%d", p->u.c.v.i);
        } 
        else
                assert(p->scope != CONSTANTS || isptr(p->type) || isfloat(p->type)),
                        p->x.name = p->name;
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
        if (p->u.seg == BSS) {
                if (p->sclass == STATIC || Aflag >= 2)
                        print(".lcomm %s,%d\n", p->x.name, p->type->size);
                else
                        print( ".comm %s,%d\n", p->x.name, p->type->size);
        } else {
                if (p->u.seg == DATA
                && (p->type->size == 0 || p->type->size > gnum))
                        print(".data\n");
                else if (p->u.seg == DATA)
                        print(".sdata\n");
                print(".align %c\n", ".01.2...3"[p->type->align]);
                print("%s:\n", p->x.name);
        }
}
static void segment(int n) {
        cseg = n;
        switch (n) {
        case CODE: print(".text\n");  break;
        case LIT:  print(".rdata\n"); break;
        }
}
static void space(int n) {
        if (cseg != BSS)
                print(".space %d\n", n);
}

static void blkloop(int dreg, int doff, int sreg, int soff, int size, int tmps[]) {
        int lab = genlabel(1);

        print("addu $%d,$%d,%d\n", sreg, sreg, size&~7);
        print("addu $%d,$%d,%d\n", tmps[2], dreg, size&~7);
        blkcopy(tmps[2], doff, sreg, soff, size&7, tmps);
        print("L.%d:\n", lab);
        print("addu $%d,$%d,%d\n", sreg, sreg, -8);
        print("addu $%d,$%d,%d\n", tmps[2], tmps[2], -8);
        blkcopy(tmps[2], doff, sreg, soff, 8, tmps);
        print("bltu $%d,$%d,L.%d\n", dreg, tmps[2], lab);
}
static void blkfetch(int size, int off, int reg, int tmp) {
        assert(size == 1 || size == 2 || size == 4);
        if (size == 1)
                print("lbu $%d,%d($%d)\n",  tmp, off, reg);
        else if (salign >= size && size == 2)
                print("lhu $%d,%d($%d)\n",  tmp, off, reg);
        else if (salign >= size)
                print("lw $%d,%d($%d)\n",   tmp, off, reg);
        else if (size == 2)
                print("ulhu $%d,%d($%d)\n", tmp, off, reg);
        else
                print("ulw $%d,%d($%d)\n",  tmp, off, reg);
}
static void blkstore(int size, int off, int reg, int tmp) {
        if (size == 1)
                print("sb $%d,%d($%d)\n",  tmp, off, reg);
        else if (dalign >= size && size == 2)
                print("sh $%d,%d($%d)\n",  tmp, off, reg);
        else if (dalign >= size)
                print("sw $%d,%d($%d)\n",  tmp, off, reg);
        else if (size == 2)
                print("ush $%d,%d($%d)\n", tmp, off, reg);
        else
                print("usw $%d,%d($%d)\n", tmp, off, reg);
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
        8, 8, 0,  /* double */
        8, 8, 1,  /* long double */
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

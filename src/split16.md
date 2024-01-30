%{
#include "split16.md.pre.c"
%}
%start stmt
%term CNSTF4=4113 CNSTF8=8209
%term CNSTI1=1045 CNSTI2=2069 CNSTI4=4117
%term CNSTP2=2071
%term CNSTU1=1046 CNSTU2=2070 CNSTU4=4118

%term ARGB=41
%term ARGF4=4129 ARGF8=8225
%term ARGI2=2085 ARGI4=4133
%term ARGP2=2087
%term ARGU2=2086 ARGU4=4134

%term ASGNB=57
%term ASGNF4=4145 ASGNF8=8241
%term ASGNI1=1077 ASGNI2=2101 ASGNI4=4149
%term ASGNP2=2103
%term ASGNU1=1078 ASGNU2=2102 ASGNU4=4150

%term INDIRB=73
%term INDIRF4=4161 INDIRF8=8257
%term INDIRI1=1093 INDIRI2=2117 INDIRI4=4165
%term INDIRP2=2119
%term INDIRU1=1094 INDIRU2=2118 INDIRU4=4166

%term CVFF4=4209 CVFF8=8305
%term CVFI2=2165 CVFI4=4213

%term CVIF4=4225 CVIF8=8321
%term CVII1=1157 CVII2=2181 CVII4=4229
%term CVIU1=1158 CVIU2=2182 CVIU4=4230

%term CVPU2=2198

%term CVUI1=1205 CVUI2=2229 CVUI4=4277
%term CVUP2=2231
%term CVUU1=1206 CVUU2=2230 CVUU4=4278

%term NEGF4=4289 NEGF8=8385
%term NEGI2=2245 NEGI4=4293

%term CALLB=217
%term CALLF4=4305 CALLF8=8401
%term CALLI2=2261 CALLI4=4309
%term CALLP2=2263
%term CALLU2=2262 CALLU4=4310
%term CALLV=216

%term RETF4=4337 RETF8=8433
%term RETI2=2293 RETI4=4341
%term RETP2=2295
%term RETU2=2294 RETU4=4342
%term RETV=248

%term ADDRGP2=2311

%term ADDRFP2=2327

%term ADDRLP2=2343

%term ADDF4=4401 ADDF8=8497
%term ADDI2=2357 ADDI4=4405
%term ADDP2=2359
%term ADDU2=2358 ADDU4=4406

%term SUBF4=4417 SUBF8=8513
%term SUBI2=2373 SUBI4=4421
%term SUBP2=2375
%term SUBU2=2374 SUBU4=4422

%term LSHI2=2389 LSHI4=4437
%term LSHU2=2390 LSHU4=4438

%term MODI2=2405 MODI4=4453
%term MODU2=2406 MODU4=4454

%term RSHI2=2421 RSHI4=4469
%term RSHU2=2422 RSHU4=4470

%term BANDI2=2437 BANDI4=4485
%term BANDU2=2438 BANDU4=4486

%term BCOMI2=2453 BCOMI4=4501
%term BCOMU2=2454 BCOMU4=4502

%term BORI2=2469 BORI4=4517
%term BORU2=2470 BORU4=4518

%term BXORI2=2485 BXORI4=4533
%term BXORU2=2486 BXORU4=4534

%term DIVF4=4545 DIVF8=8641
%term DIVI2=2501 DIVI4=4549
%term DIVU2=2502 DIVU4=4550

%term MULF4=4561 MULF8=8657
%term MULI2=2517 MULI4=4565
%term MULU2=2518 MULU4=4566

%term EQF4=4577 EQF8=8673
%term EQI2=2533 EQI4=4581
%term EQU2=2534 EQU4=4582

%term GEF4=4593 GEF8=8689
%term GEI2=2549 GEI4=4597
%term GEU2=2550 GEU4=4598

%term GTF4=4609 GTF8=8705
%term GTI2=2565 GTI4=4613
%term GTU2=2566 GTU4=4614

%term LEF4=4625 LEF8=8721
%term LEI2=2581 LEI4=4629
%term LEU2=2582 LEU4=4630

%term LTF4=4641 LTF8=8737
%term LTI2=2597 LTI4=4645
%term LTU2=2598 LTU4=4646

%term NEF4=4657 NEF8=8753
%term NEI2=2613 NEI4=4661
%term NEU2=2614 NEU4=4662

%term JUMPV=584

%term LABELV=600


%term VREGP=711
%term LOADI4=4325
%term LOADU4=4326
%term LOADI2=2277
%term LOADU2=2278
%term LOADP2=2279
%term LOADF4=4321
%term LOADB=233
%term LOADF8=8417
%term LOADI1=1253
%term LOADU1=1254

%term POPI2=4821
%term POPU2=4822
%term POPI1=2773
%term POPU1=2774
%term POPP1=2775
%term POPF2=4817
%term POPF4=8913
%term POPB=729


%%
stmt: v "%a"

v: ARGF4(f) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGF8(f) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGI2(i) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGI4(i) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGP2(p) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGU2(u) "?%I rpush.sp\n%0\n" splitCost(1, 1)
v: ARGU4(u) "?%I rpush.sp\n%0\n" splitCost(1, 1)

v: ASGNF4(p,f) "%0\n%1%Lstore.32\n" splitCost(1, 1)
v: ASGNF8(p,f) "%0\n%1%Lstore.64\n" splitCost(1, 1)
v: ASGNI1(p,i) "%0\n%1%Lstore.s8\n" splitCost(1, 1)
v: ASGNI2(p,i) "%0\n%1%Lstore.16\n" splitCost(1, 1)
v: ASGNI4(p,i) "%0\n%1%Lstore.32\n" splitCost(1, 1)
v: ASGNP2(p,p) "%0\n%1%Lstore.16\n" splitCost(1, 1)
v: ASGNU1(p,u) "%0\n%1%Lstore.u8\n" splitCost(1, 1)
v: ASGNU2(p,u) "%0\n%1%Lstore.16\n" splitCost(1, 1)
v: ASGNU4(p,u) "%0\n%1%Lstore.32\n" splitCost(1, 1)

v: EQF4(f,f) "%0\n%1%LEQF4%T%a\n" splitCost(3, 1)
v: EQF8(f,f) "%0\n%1%LEQF8%T%a\n" splitCost(3, 1)
v: EQI2(i,i) "%0\n%1%LEQI2%T%a\n" splitCost(3, 1)
v: EQI4(i,i) "%0\n%1%LEQI4%T%a\n" splitCost(3, 1)
v: EQU2(u,u) "%0\n%1%LEQU2%T%a\n" splitCost(3, 1)
v: EQU4(u,u) "%0\n%1%LEQU4%T%a\n" splitCost(3, 1)
v: GEF4(f,f) "%0\n%1%LGEF4%T%a\n" splitCost(3, 1)
v: GEF8(f,f) "%0\n%1%LGEF8%T%a\n" splitCost(3, 1)
v: GEI2(i,i) "%0\n%1%LGEI2%T%a\n" splitCost(3, 1)
v: GEI4(i,i) "%0\n%1%LGEI4%T%a\n" splitCost(3, 1)
v: GEU2(u,u) "%0\n%1%LGEU2%T%a\n" splitCost(3, 1)
v: GEU4(u,u) "%0\n%1%LGEU4%T%a\n" splitCost(3, 1)
v: GTF4(f,f) "%0\n%1%LGTF4%T%a\n" splitCost(3, 1)
v: GTF8(f,f) "%0\n%1%LGTF8%T%a\n" splitCost(3, 1)
v: GTI2(i,i) "%0\n%1%LGTI2%T%a\n" splitCost(3, 1)
v: GTI4(i,i) "%0\n%1%LGTI4%T%a\n" splitCost(3, 1)
v: GTU2(u,u) "%0\n%1%LGTU2%T%a\n" splitCost(3, 1)
v: GTU4(u,u) "%0\n%1%LGTU4%T%a\n" splitCost(3, 1)
v: LEF4(f,f) "%0\n%1%LLEF4%T%a\n" splitCost(3, 1)
v: LEF8(f,f) "%0\n%1%LLEF8%T%a\n" splitCost(3, 1)
v: LEI2(i,i) "%0\n%1%LLEI2%T%a\n" splitCost(3, 1)
v: LEI4(i,i) "%0\n%1%LLEI4%T%a\n" splitCost(3, 1)
v: LEU2(u,u) "%0\n%1%LLEU2%T%a\n" splitCost(3, 1)
v: LEU4(u,u) "%0\n%1%LLEU4%T%a\n" splitCost(3, 1)
v: LTF4(f,f) "%0\n%1%LLTF4%T%a\n" splitCost(3, 1)
v: LTF8(f,f) "%0\n%1%LLTF8%T%a\n" splitCost(3, 1)
v: LTI2(i,i) "%0\n%1%LLTI2%T%a\n" splitCost(3, 1)
v: LTI4(i,i) "%0\n%1%LLTI4%T%a\n" splitCost(3, 1)
v: LTU2(u,u) "%0\n%1%LLTU2%T%a\n" splitCost(3, 1)
v: LTU4(u,u) "%0\n%1%LLTU4%T%a\n" splitCost(3, 1)
v: NEF4(f,f) "%0\n%1%LNEF4%T%a\n" splitCost(3, 1)
v: NEF8(f,f) "%0\n%1%LNEF8%T%a\n" splitCost(3, 1)
v: NEI2(i,i) "%0\n%1%LNEI2%T%a\n" splitCost(3, 1)
v: NEI4(i,i) "%0\n%1%LNEI4%T%a\n" splitCost(3, 1)
v: NEU2(u,u) "%0\n%1%LNEU2%T%a\n" splitCost(3, 1)
v: NEU4(u,u) "%0\n%1%LNEU4%T%a\n" splitCost(3, 1)

v: EQI2(i,cnst_zero) "%0%Lbr.eqz%T%a\n" splitCost(3, 1)
v: NEI2(i,cnst_zero) "%0%Lbr.nez%T%a\n" splitCost(3, 1)

v: JUMPV(p) "%0%LJUMPV\n" 10
v: LABELV "%<%I%a%>:\n"

v: RETF4(f) "%0%Lreturn.32\n"
v: RETF8(f) "%0%Lreturn.64\n" 
v: RETI2(i) "%0%Lreturn.16\n"
v: RETI4(i) "%0%Lreturn.32\n"
v: RETP2(p) "%0%Lreturn.16\n"
v: RETU2(u) "%0%Lreturn.16\n"
v: RETU4(u) "%0%Lreturn.32\n"
v: RETV "RETV\n"




f: CVFF8(f) "%I rpush.sp\n%0%Lcall%Tcv_f4_to_f8" ifCost(cvSrcSize(a) == 4, 1, 1)
i: CVFI2(f) "%I rpush.sp\n%0%Lcall%Tcv_f4_to_i2" ifCost(cvSrcSize(a) == 4, 1, 1)
i: CVFI4(f) "%I rpush.sp\n%0%Lcall%Tcv_f4_to_i4" ifCost(cvSrcSize(a) == 4, 1, 1)
f: CVFF4(f) "%I rpush.sp\n%0%Lcall%Tcv_f8_to_f4" ifCost(cvSrcSize(a) == 8, 1, 1)
i: CVFI2(f) "%I rpush.sp\n%0%Lcall%Tcv_f8_to_i2" ifCost(cvSrcSize(a) == 8, 1, 1)
i: CVFI4(f) "%I rpush.sp\n%0%Lcall%Tcv_f8_to_i4" ifCost(cvSrcSize(a) == 8, 1, 1)

i: CVII2(i) "%0%Lsex ; cv_i1_to_i2" ifCost(cvSrcSize(a) == 1, 1, 1)
f: CVIF4(i) "%I rpush.sp\n%0%Lcall%Tcv_i2_to_f4" ifCost(cvSrcSize(a) == 2, 1, 1)
f: CVIF8(i) "%I rpush.sp\n%0%Lcal%T[.16]%Tcv_i2_to_f8" ifCost(cvSrcSize(a) == 2, 1, 1)
i: CVII1(i) "%0 ; cv_i2_to_i1" ifCost(cvSrcSize(a) == 2, 0, 0)
i: CVII4(i) "%I rpush.sp\n%0%Lcall%Tcv_i2_to_i4" ifCost(cvSrcSize(a) == 2, 1, 1)
u: CVIU2(i) "%0 ; cv_i2_to_u2" ifCost(cvSrcSize(a) == 2, 0, 0)
u: CVIU4(i) "%I rpush.sp\n%0%Lcall%Tcv_i2_to_u4" ifCost(cvSrcSize(a) == 2, 1, 1)
f: CVIF4(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_f4" ifCost(cvSrcSize(a) == 4, 1, 1)
f: CVIF8(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_f8" ifCost(cvSrcSize(a) == 4, 1, 1)
i: CVII1(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_i1" ifCost(cvSrcSize(a) == 4, 1, 1)
i: CVII2(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_i2" ifCost(cvSrcSize(a) == 4, 1, 1)
u: CVIU2(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_u2" ifCost(cvSrcSize(a) == 4, 1, 1)
u: CVIU4(i) "%I rpush.sp\n%0%Lcall%Tcv_i4_to_u4" ifCost(cvSrcSize(a) == 4, 1, 1)

i: CVUI2(u) "%0%Lpush.16%T0xff%Land.16 ; cv_u1_to_i2" ifCost(cvSrcSize(a) == 1, 1, 1)
i: CVUI2(u) "%0 ; cv_u2_to_i2" ifCost(cvSrcSize(a) == 2, 0, 0)
i: CVUI4(u) "%I rpush.sp\n%0%Lcall%Tcv_u2_to_i4" ifCost(cvSrcSize(a) == 2, 1, 1)
p: CVUP2(u) "%0 ; cv_u2_to_p2" ifCost(cvSrcSize(a) == 2, 0, 0)
u: CVUU1(u) "%0 ; cv_u2_to_u1" ifCost(cvSrcSize(a) == 2, 0, 0)
u: CVUU4(u) "%I rpush.sp\n%0%Lcall%Tcv_u2_to_u4" ifCost(cvSrcSize(a) == 2, 1, 1)
i: CVUI2(u) "%I rpush.sp\n%0%Lcall%Tcv_u4_to_i2" ifCost(cvSrcSize(a) == 4, 1, 1)
i: CVUI4(u) "%I rpush.sp\n%0%Lcall%Tcv_u4_to_i4" ifCost(cvSrcSize(a) == 4, 1, 1)
u: CVUU1(u) "%I rpush.sp\n%0%Lcall%Tcv_u4_to_u1" ifCost(cvSrcSize(a) == 4, 1, 1)
u: CVUU2(u) "%I rpush.sp\n%0%Lcall%Tcv_u4_to_u2" ifCost(cvSrcSize(a) == 4, 1, 1)

f: ADDF4(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tadd_f32"  splitCost(1, 1)
f: ADDF8(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tadd_f64"  splitCost(1, 1)
i: ADDI2(i,i) "%0\n%1%Ladd.16" splitCost(1, 1)
i: ADDI4(i,i) "%0\n%1%Ladd.32" splitCost(1, 1)
u: ADDU2(u,u) "%0\n%1%Ladd.16" splitCost(1, 1)
u: ADDU4(u,u) "%0\n%1%Ladd.32" splitCost(1, 1)
p: ADDP2(i,p) "%0\n%1%Ladd.16 ; xxa" splitCost(1, 1)
p: ADDP2(p,i) "%0\n%1%Ladd.16 ; xxb" splitCost(1, 1)
p: ADDP2(p,u) "%0\n%1%Ladd.16 ; xxc" splitCost(1, 1)
p: ADDP2(u,p) "%0\n%1%Ladd.16 ; xxd" splitCost(1, 1)

f: SUBF4(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tsub_f32" 10
f: SUBF8(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tsub_f64" 10
i: SUBI2(i,i) "%0\n%1%Lsub.16" splitCost(1, 1)
i: SUBI4(i,i) "%0\n%1%Lsub.32" splitCost(1, 1)
u: SUBU2(u,u) "%0\n%1%Lsub.16" splitCost(1, 1)
u: SUBU4(u,u) "%0\n%1%Lsub.32" splitCost(1, 1)
p: SUBP2(p,i) "%0\n%1%Lsub.16" splitCost(1, 1)
p: SUBP2(p,u) "%0\n%1%Lsub.16" splitCost(1, 1)

i: BANDI2(i,i) "%0\n%1%Land.16" splitCost(1, 1)
i: BANDI4(i,i) "%0\n%1%Land.32" splitCost(1, 1)
u: BANDU2(u,u) "%0\n%1%Land.16" splitCost(1, 1)
u: BANDU4(u,u) "%0\n%1%Land.16" splitCost(1, 1)

i: BORI2(i,i) "%0%Lor.16" splitCost(1, 1)
i: BORI4(i,i) "%0%Lor.32" splitCost(1, 1)
u: BORU2(u,u) "%0%Lor.16" splitCost(1, 1)
u: BORU4(u,u) "%0%Lor.32" splitCost(1, 1)

i: BXORI2(i,i) "%0%Lxor.16" splitCost(1, 1)
i: BXORI4(i,i) "%0%Lxor.32" splitCost(1, 1)
u: BXORU2(u,u) "%0%Lxor.16" splitCost(1, 1)
u: BXORU4(u,u) "%0%Lxor.32" splitCost(1, 1)

i: LSHI2(i,i) "%0\n%1%Llsh.16"
i: LSHI4(i,i) "%I rpush.sp\n%0\n%1%Lcall%Tlsh_32"
u: LSHU2(u,i) "%0\n%1%Llsh.16"
u: LSHU4(u,i) "%I rpush.sp\n%0\n%1%Lcall%Tlsh_32"

i: RSHI2(i,i) "%0\n%1%Lrsh.16"
i: RSHI4(i,i) "%I rpush.sp\n%0\n%1%Lcall%Trsh_32"
u: RSHU2(u,i) "%0\n%1%Lrsh.16"
u: RSHU4(u,i) "%I rpush.sp\n%0\n%1%Lcall%Trsh_32"

f: NEGF4(f) "%I rpush.sp\n%0%Lcall%Tneg_f4"
f: NEGF8(f) "%I rpush.sp\n%0%Lcall%Tneg_f8"
i: NEGI2(i) "%0%Lneg.16"
i: NEGI4(i) "%0%Lneg.32"

i: BCOMI2(i) "%0%Lpush[.16]%T-1%Lxor.16"
i: BCOMI4(i) "%I rpush.sp\n%0%Lcall%Tbcom_32"
u: BCOMU2(u) "%0%Lpush[.16]%T-1%Lxor.16"
u: BCOMU4(u) "%I rpush.sp\n%0%Lcall%Tbcom_32"

f: DIVF4(f,f) "%I rpush.sp\n%0%Lcall%Tdiv_f32"
f: DIVF8(f,f) "%I rpush.sp\n%0%Lcall%Tdiv_f64"
i: DIVI2(i,i) "%I rpush.sp\n%0%Lcall%Tdiv_i16"
i: DIVI4(i,i) "%I rpush.sp\n%0%Lcall%Tdiv_i32"
u: DIVU2(u,u) "%I rpush.sp\n%0%Lcall%Tdiv_u16"
u: DIVU4(u,u) "%I rpush.sp\n%0%Lcall%Tdiv_u32"

f: MULF4(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tmul_f32"
f: MULF8(f,f) "%I rpush.sp\n%0\n%1%Lcall%Tmul_f64"
i: MULI2(i,i) "%I rpush.sp\n%0\n%1%Lcall%Tmul_i16"
i: MULI4(i,i) "%I rpush.sp\n%0\n%1%Lcall%Tmul_i32"
u: MULU2(u,u) "%I rpush.sp\n%0\n%1%Lcall%Tmul_u16"
u: MULU4(u,u) "%I rpush.sp\n%0\n%1%Lcall%Tmul_u32"

i: MODI2(i,i) "%I rpush.sp\n%0%Lcall%Tmod_i16"
i: MODI4(i,i) "%I rpush.sp\n%0%Lcall%Tmod_i32"
u: MODU2(u,u) "%I rpush.sp\n%0%Lcall%Tmod_u16"
u: MODU4(u,u) "%I rpush.sp\n%0%Lcall%Tmod_u32"

i: CNSTI1 "%Ipush.16%T%a" splitCost(2, 1)
u: CNSTU1 "%Ipush.u8%T%a" splitCost(2, 1)
i: CNSTI2 "%Ipush.16%T%a" splitCost(2, 1)
u: CNSTU2 "%Ipush.16%T%a" splitCost(3, 1)
p: CNSTP2 "%Ipush.16%T%a" splitCost(3, 1)
i: CNSTI4 "%Ipush.32%T%a" splitCost(5, 1)
u: CNSTU4 "%Ipush.32%T%a" splitCost(5, 1)
f: CNSTF4 "%Ipush.32%T%a" splitCost(5, 1)
f: CNSTF8 "%Ipush.64%T%a" splitCost(9, 1)

i: CNSTI1 "%Ipush.s8%T%a" ifCost(inRange(a, 0, 255), 2, 1)
i: CNSTI2 "%Ipush.s8%T%a" ifCost(inRange(a, 0, 255), 2, 1)

cnst_zero: CNSTI2 "%a" range(a,0,0)
conS8: CNSTI2 "%a" inRange(a, 0, 255)
conS16: CNSTI2 "%a"


stmt: POPI2(i) "%0%Lpop.32\n"
stmt: POPU2(u) "%0%Lpop.32\n"
stmt: POPI1(i) "%0%Lpop.16\n"
stmt: POPU1(u) "%0%Lpop.16\n"
stmt: POPP1(p) "%0%Lpop.16\n"
stmt: POPF2(f) "%0%Lpop.32\n"
stmt: POPF4(f) "%0%Lpop.64\n"

f: CALLF4(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
f: CALLF8(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
i: CALLI2(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
i: CALLI4(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
p: CALLP2(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
u: CALLU2(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)
u: CALLU4(p) "?%I rpush.sp\n%0%Lcall" splitCost(0,0)

f: CALLF4(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
f: CALLF8(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
i: CALLI2(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
i: CALLI4(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
p: CALLP2(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
u: CALLU2(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)
u: CALLU4(g16) "?%I rpush.sp\n%Icall%T%0" splitCost(0,0)

v: CALLB(p,p) "ERROR ; Not expecting CALLB(p,p)\n" 100
v: CALLB(g16,p) "ERROR ; Not expecting CALLB(g16,p)\n" 100
v: CALLV(p) "?%I rpush.sp\n%0%Lcall\n" 1
v: CALLV(g16) "?%I rpush.sp\n%Icall%T%0\n" 0

f: INDIRF4(p) "%0%Lload.32" 1
f: INDIRF8(p) "%0%Lload.64" 1
i: INDIRI1(p) "%0%Lload.s8" 1
i: INDIRI2(p) "%0%Lload.16" 1
i: INDIRI4(p) "%0%Lload.32" 1
p: INDIRP2(p) "%0%Lload.16" 1
u: INDIRU1(p) "%0%Lload.u8" 1
u: INDIRU2(p) "%0%Lload.16" 1
u: INDIRU4(p) "%0%Lload.32" 1




f: INDIRF4(ADDP2(p, conS8)) "%0%Lload.32%Ttos[%1]" splitCost(2, 1)
f: INDIRF8(ADDP2(p, conS8)) "%0%Lload.64%Ttos[%1]" splitCost(2, 1)
i: INDIRI1(ADDP2(p, conS8)) "%0%Lload.s8%Ttos[%1]" splitCost(2, 1)
i: INDIRI2(ADDP2(p, conS8)) "%0%Lload.16%Ttos[%1]" splitCost(2, 1)
i: INDIRI4(ADDP2(p, conS8)) "%0%Lload.32%Ttos[%1]" splitCost(2, 1)
p: INDIRP2(ADDP2(p, conS8)) "%0%Lload.16%Ttos[%1]" splitCost(2, 1)
u: INDIRU1(ADDP2(p, conS8)) "%0%Lload.u8%Ttos[%1]" splitCost(2, 1)
u: INDIRU2(ADDP2(p, conS8)) "%0%Lload.16%Ttos[%1]" splitCost(2, 1)
u: INDIRU4(ADDP2(p, conS8)) "%0%Lload.32%Ttos[%1]" splitCost(2, 1)

f: INDIRF4(ADDP2(p, conS16)) "%0%Lload.32%Ttos[%1]" splitCost(3, 1)
f: INDIRF8(ADDP2(p, conS16)) "%0%Lload.64%Ttos[%1]" splitCost(3, 1)
i: INDIRI1(ADDP2(p, conS16)) "%0%Lload.s8%Ttos[%1]" splitCost(3, 1)
i: INDIRI2(ADDP2(p, conS16)) "%0%Lload.16%Ttos[%1]" splitCost(3, 1)
i: INDIRI4(ADDP2(p, conS16)) "%0%Lload.32%Ttos[%1]" splitCost(3, 1)
p: INDIRP2(ADDP2(p, conS16)) "%0%Lload.16%Ttos[%1]" splitCost(3, 1)
u: INDIRU1(ADDP2(p, conS16)) "%0%Lload.u8%Ttos[%1]" splitCost(3, 1)
u: INDIRU2(ADDP2(p, conS16)) "%0%Lload.16%Ttos[%1]" splitCost(3, 1)
u: INDIRU4(ADDP2(p, conS16)) "%0%Lload.32%Ttos[%1]" splitCost(3, 1)





v: ARGB(INDIRB(p)) "?%I rpush.sp\n%0%LloadBytes%T%a\n" ifCost(argNo(a)==0, 4, 1)
v: ARGB(INDIRB(p)) "?%I rpush.sp\n%0%LloadBytes%T%a\n" ifCost(argNo(a)!=0, 3, 1)

v: ASGNB(p,INDIRB(p)) "%0\n%1%Lmemcopy%T%a\n" ifCost(inRange(a, 0, 255), 2, 1)
v: ASGNB(p,INDIRB(p)) "%0\n%1%Lmemcopy%T%a\n" splitCost(3, 1)

p: ADDRFP2 "%Ipushea%Tfp[%a]" splitCost(3, 1)
p: ADDRLP2 "%Ipushea%Tfp[%a]" splitCost(3, 1)
p: ADDRGP2 "%Ipushea%Tdp[%a]" splitCost(3, 1)

p: ADDRFP2 "%Ipushea%Tfp[%a]" ifCost(inRange(a, -128, 127), 2, 1)
p: ADDRLP2 "%Ipushea%Tfp[%a]" ifCost(inRange(a, -128, 127), 2, 1)


v: JUMPV(g16) "%Ijmp%T%0\n" 10

i: CVUI2(INDIRU1(p)) "%0%Lload.8 ; cv_u1_to_i2"  ifCost(cvSrcSize(a) == 1, 1, 1)
i: CVUI2(u1) "%0 ; cv_u1_to_i2" ifCost(cvSrcSize(a) == 1, 0, 0)
u1: CVUU1(u) "%0%Lpush.16%T0xff%Land.16 ; cv_u2_to_u1" ifCost(cvSrcSize(a) == 2, 3, 1)
u: CVPU2(p) "%<%0%> ; cv_p2_to_u2" ifCost(cvSrcSize(a) == 2, 0, 0)

g16: ADDRGP2 "%a" 0
fp16: ADDRFP2 "%a" 0
fp16: ADDRLP2 "%a" 0

fp8: ADDRFP2 "%a" range(a, -128, 127)
fp8: ADDRLP2 "%a" range(a, -128, 127)


i: CVII1(i) "%0%Lswap.16 ; assumes little endian%Lpop.16 ; cv_i4_to_i1" ifCost(cvSrcSize(a) == 2, 1, 1)

i: CVII2(INDIRI1(fp8)) "%Ipush.s8%Tfp[%0] ; cv_i1_i2" splitCost(2, 1)
i: CVUI2(INDIRU1(fp8)) "%Ipush.u8%Tfp[%0] ; cv_u1_i2" splitCost(2, 1)
i: CVUI2(INDIRU2(fp8)) "%Ipush.16%Tfp[%0] ; cv_u2_i2" splitCost(2, 1)
i: CVUI1(INDIRU2(fp8)) "%Ipush.16%Tfp[%0] ; cv_u2_i1" splitCost(2, 1)

i: CVII2(INDIRI1(fp16)) "%Ipush.s8%Tfp[%0] ; cv_i1_i2" splitCost(3, 1)
i: CVUI2(INDIRU1(fp16)) "%Ipush.u8%Tfp[%0] ; cv_u1_i2" splitCost(3, 1)
i: CVUI2(INDIRU2(fp16)) "%Ipush.16%Tfp[%0] ; cv_u2_i2" splitCost(3, 1)
i: CVUI1(INDIRU2(fp16)) "%Ipush.16%Tfp[%0] ; cv_u2_i1" splitCost(3, 1)

i: CVII2(INDIRI1(p)) "%0%Lload.s8 ; cv_i1_to_i2" splitCost(1, 1)

i: INDIRI1(fp8) "%Ipush.s8%Tfp[%0]" splitCost(2, 1)
u: INDIRU1(fp8) "%Ipush.u8%Tfp[%0]" splitCost(2, 1)
i: INDIRI2(fp8) "%Ipush.16%Tfp[%0]" splitCost(2, 1)
u: INDIRU2(fp8) "%Ipush.16%Tfp[%0]" splitCost(2, 1)
p: INDIRP2(fp8) "%Ipush.16%Tfp[%0]" splitCost(2, 1)
i: INDIRI4(fp8) "%Ipush.32%Tfp[%0]" splitCost(2, 1)
u: INDIRU4(fp8) "%Ipush.32%Tfp[%0]" splitCost(2, 1)
f: INDIRF4(fp8) "%Ipush.32%Tfp[%0]" splitCost(2, 1)
f: INDIRF8(fp8) "%Ipush.64%Tfp[%0]" splitCost(2, 1)

i: INDIRI1(fp16) "%Ipush.s8%Tfp[%0]" splitCost(3, 1)
u: INDIRU1(fp16) "%Ipush.u8%Tfp[%0]" splitCost(3, 1)
i: INDIRI2(fp16) "%Ipush.16%Tfp[%0]" splitCost(3, 1)
u: INDIRU2(fp16) "%Ipush.16%Tfp[%0]" splitCost(3, 1)
p: INDIRP2(fp16) "%Ipush.16%Tfp[%0]" splitCost(3, 1)
i: INDIRI4(fp16) "%Ipush.32%Tfp[%0]" splitCost(3, 1)
u: INDIRU4(fp16) "%Ipush.32%Tfp[%0]" splitCost(3, 1)
f: INDIRF4(fp16) "%Ipush.32%Tfp[%0]" splitCost(3, 1)
f: INDIRF8(fp16) "%Ipush.64%Tfp[%0]" splitCost(3, 1)


i: INDIRI1(g16) "%Ipush.s8%Tdp[%0]" splitCost(3, 1)
u: INDIRU1(g16) "%Ipush.u8%Tdp[%0]" splitCost(3, 1)
i: INDIRI2(g16) "%Ipush.16%Tdp[%0]" splitCost(3, 1)
u: INDIRU2(g16) "%Ipush.16%Tdp[%0]" splitCost(3, 1)
p: INDIRP2(g16) "%Ipush.16%Tdp[%0]" splitCost(3, 1)
i: INDIRI4(g16) "%Ipush.32%Tdp[%0]" splitCost(3, 1)
u: INDIRU4(g16) "%Ipush.32%Tdp[%0]" splitCost(3, 1)
f: INDIRF4(g16) "%Ipush.32%Tdp[%0]" splitCost(3, 1)
f: INDIRF8(g16) "%Ipush.64%Tdp[%0]" splitCost(3, 1)

v: ASGNI1(fp8,i) "%1%Lstore.s8%Tfp[%0]\n" splitCost(2, 1)
v: ASGNU1(fp8,u) "%1%Lstore.u8%Tfp[%0]\n" splitCost(2, 1)
v: ASGNI2(fp8,i) "%1%Lstore.16%Tfp[%0]\n" splitCost(2, 1)
v: ASGNU2(fp8,u) "%1%Lstore.16%Tfp[%0]\n" splitCost(2, 1)
v: ASGNP2(fp8,p) "%1%Lstore.16%Tfp[%0]\n" splitCost(2, 1)
v: ASGNI4(fp8,i) "%1%Lstore.32%Tfp[%0]\n" splitCost(2, 1)
v: ASGNU4(fp8,u) "%1%Lstore.32%Tfp[%0]\n" splitCost(2, 1)
v: ASGNF4(fp8,f) "%1%Lstore.32%Tfp[%0]\n" splitCost(2, 1)
v: ASGNF8(fp8,f) "%1%Lstore.64%Tfp[%0]\n" splitCost(2, 1)

v: ASGNI1(fp16,i) "%1%Lstore.s8%Tfp[%0]\n" splitCost(3, 1)
v: ASGNU1(fp16,u) "%1%Lstore.u8%Tfp[%0]\n" splitCost(3, 1)
v: ASGNI2(fp16,i) "%1%Lstore.16%Tfp[%0]\n" splitCost(3, 1)
v: ASGNU2(fp16,u) "%1%Lstore.16%Tfp[%0]\n" splitCost(3, 1)
v: ASGNP2(fp16,p) "%1%Lstore.16%Tfp[%0]\n" splitCost(3, 1)
v: ASGNI4(fp16,i) "%1%Lstore.32%Tfp[%0]\n" splitCost(3, 1)
v: ASGNU4(fp16,u) "%1%Lstore.32%Tfp[%0]\n" splitCost(3, 1)
v: ASGNF4(fp16,f) "%1%Lstore.32%Tfp[%0]\n" splitCost(3, 1)
v: ASGNF8(fp16,f) "%1%Lstore.64%Tfp[%0]\n" splitCost(3, 1)

v: ASGNI1(g16,i) "%1%Lstore.s8%Tdp[%0]\n" splitCost(3, 1)
v: ASGNU1(g16,u) "%1%Lstore.u8%Tdp[%0]\n" splitCost(3, 1)
v: ASGNI2(g16,i) "%1%Lstore.16%Tdp[%0]\n" splitCost(3, 1)
v: ASGNU2(g16,u) "%1%Lstore.16%Tdp[%0]\n" splitCost(3, 1)
v: ASGNP2(g16,p) "%1%Lstore.16%Tdp[%0]\n" splitCost(3, 1)
v: ASGNI4(g16,i) "%1%Lstore.32%Tdp[%0]\n" splitCost(3, 1)
v: ASGNU4(g16,u) "%1%Lstore.32%Tdp[%0]\n" splitCost(3, 1)
v: ASGNF4(g16,f) "%1%Lstore.32%Tdp[%0]\n" splitCost(3, 1)
v: ASGNF8(g16,f) "%1%Lstore.64%Tdp[%0]\n" splitCost(3, 1)

i: INDIRI1(INDIRP2(fp8)) "%Ipush.s8%T*fp[%0]" splitCost(2, 1)
u: INDIRU1(INDIRP2(fp8)) "%Ipush.u8%T*fp[%0]" splitCost(2, 1)
i: INDIRI2(INDIRP2(fp8)) "%Ipush.16%T*fp[%0]" splitCost(2, 1)
u: INDIRU2(INDIRP2(fp8)) "%Ipush.16%T*fp[%0]" splitCost(2, 1)
p: INDIRP2(INDIRP2(fp8)) "%Ipush.16%T*fp[%0]" splitCost(2, 1)
i: INDIRI4(INDIRP2(fp8)) "%Ipush.32%T*fp[%0]" splitCost(2, 1)
u: INDIRU4(INDIRP2(fp8)) "%Ipush.32%T*fp[%0]" splitCost(2, 1)
f: INDIRF4(INDIRP2(fp8)) "%Ipush.32%T*fp[%0]" splitCost(2, 1)
f: INDIRF8(INDIRP2(fp8)) "%Ipush.64%T*fp[%0]" splitCost(2, 1)

i: INDIRI1(INDIRP2(fp16)) "%Ipush.s8%T*fp[%0]" splitCost(3, 1)
u: INDIRU1(INDIRP2(fp16)) "%Ipush.u8%T*fp[%0]" splitCost(3, 1)
i: INDIRI2(INDIRP2(fp16)) "%Ipush.16%T*fp[%0]" splitCost(3, 1)
u: INDIRU2(INDIRP2(fp16)) "%Ipush.16%T*fp[%0]" splitCost(3, 1)
p: INDIRP2(INDIRP2(fp16)) "%Ipush.16%T*fp[%0]" splitCost(3, 1)
i: INDIRI4(INDIRP2(fp16)) "%Ipush.32%T*fp[%0]" splitCost(3, 1)
u: INDIRU4(INDIRP2(fp16)) "%Ipush.32%T*fp[%0]" splitCost(3, 1)
f: INDIRF4(INDIRP2(fp16)) "%Ipush.32%T*fp[%0]" splitCost(3, 1)
f: INDIRF8(INDIRP2(fp16)) "%Ipush.64%T*fp[%0]" splitCost(3, 1)


v: ASGNI1(fp8,INDIRI1(fp8)) "%Icopy.s8%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNU1(fp8,INDIRU1(fp8)) "%Icopy.u8%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNI2(fp8,INDIRI2(fp8)) "%Icopy.16%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNU2(fp8,INDIRU2(fp8)) "%Icopy.16%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNP2(fp8,INDIRP2(fp8)) "%Icopy.16%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNI4(fp8,INDIRI4(fp8)) "%Icopy.32%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNU4(fp8,INDIRU4(fp8)) "%Icopy.32%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNF4(fp8,INDIRF4(fp8)) "%Icopy.32%Tfp[%0], fp[%1]\n" splitCost(3, 1)
v: ASGNF8(fp8,INDIRF8(fp8)) "%Icopy.64%Tfp[%0], fp[%1]\n" splitCost(3, 1)


v: ASGNI1(INDIRP2(fp16),i) "%1%Lstore.s8%T*fp[%0]\n" 1
v: ASGNI2(INDIRP2(fp16),i) "%1%Lstore.16%T*fp[%0]\n" 1
%%
#include "split16.md.post.c"
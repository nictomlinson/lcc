#pragma xxxxx
int y;
int *addrOfY;
int *addr2OfY;
int z;
char *const aString = "a string";
char conI1_1 = 1;
unsigned char conU1_FF = 0xFF;
short conI2_0 = 0;
short conI2_1 = 1;
unsigned short conI2_FFFF = 0xFFFF;
long conI4_0 = 0;
long conI4_1 = 1;
long long conI8 = 12345;
unsigned long conU4_FF = 0 - 1;
float conF4_v1 = 12345.56789e5;
double conf8_v2 = 12345.56789e5;
long conLong = 123456789L;
signed char conS1 = 36;
signed int cons2 = 726;

extern unsigned char c1;
extern unsigned short c2;
extern char getChar();
extern short getShort();
extern char (*getChar_f)();
extern int getInt();
extern int i1;
extern int i2;

typedef struct {
  int x;
  char c;
} byte;

byte aByte;

void doNothing() {}
void (*doNothingFptr)(void) = doNothing;
void callThroughFunctionPointer() { doNothingFptr(); }

char globalOffsetAccess() { return aString[5]; }

void (*Fptrs[2])(void);
void callThroughFunctionPointers() { Fptrs[1](); }
void assignDouble(double d) { d = 0.0; }
void charLocal() { char c = 1; }
int addCharAndInt(){char c = 1;
  int i = 64;
  return i + c;
}

char bigOffset(char *s) { return s[65280U]; }

int doNothingi2() { return 0; }
int (*doNothingi2Fptr)(void) = doNothingi2;
void callThroughFunctionPointeri2() { doNothingi2Fptr(); }

char byteC() { return aByte.c; }
char localByteC() {
  char s[10];
  byte b;

  return b.c;
}

void negate() { i1 = -i2; }
static void staticFunction(void) {}
int leftShiftI2UsingI2(int v, int s) {
  while (s) {
    v <<= 1;
    s--;
  }
  return v;
}

void localsOrder() {
  int a;
  int b;
  a = 1;
  b = (2 - a) + (2 - a) + (2 - a) + (2 - a) + (2 - a);
}

void memAddressCalc(char *c, int i) { *(c + 37 - i) = 'a'; }

void duplicateLocals() {
  static int n;
  {
    static int n;
    n = 19;
  }
  n++;
}
unsigned char unsignedCharFromFloat;
unsigned short unsignedShortFromFloat;
unsigned int unsignedIntFromFloat;
int intFromFloat;
void floatTouUsignedChar(float f) { unsignedCharFromFloat = f; }
void floatToUnsignedShort(float f) { unsignedShortFromFloat = (unsigned short)f; }
void floatToUnsignedInt(float f) { unsignedIntFromFloat = f; }
void floatToInt(float f) { intFromFloat = f; }
unsigned int i2ToU2(int i) { return i; }
unsigned char i2ToU1(int i) { return i; }
void u1ToI2(void) {
  unsigned char c;
  int i;
  i = c;
}

void i2ToI1() {
  signed char c;
  int i;
  c = i;
}

void i2ToI4() {
  int i;
  long l;
  l = i;
}

extern signed char cv_i2_to_i1_char;
extern int cv_i2_to_i1_int;

void do_cv_i2_to_u2() { int a;
  unsigned int b = a;
}

void do_cv_i2_to_i1() {
  int i;
  signed char c=i;
}

void do_cv_u1_to_i2() { unsigned char c;
  int b = c;
}

void do_cv_i4_to_i1() { long l;
  signed char c = l;
}

void do_cv_u2_to_i2() { unsigned int a;
  int b = a;
}

void signedIntToUnsignedInt() { int a = 5;
  unsigned int b;
  b = a;
}
void assignToIPlusPlus(int *intArray, int i) { intArray[i++] = i; }

void doSwitch(int i) {
  int x;
  switch (i) {
    case 1:
      x = i;
      break;
    case 2:
      x = i;
      break;
    case 7:
      x = i;
      break;
    case 8:
      x = i;
      break;
    case 9:
      x = i;
      break;
    case 16:
      x = i;
      break;
    case 17:
      x = i;
      break;
    case 18:
      x = i;
      break;
    case 19:
      x = i;
      break;
    case 20:
      x = i;
      break;
  }
}


byte adder() {
  byte b;
  b.c = 'a';
  return b;
}

void returnvoid() { return; }

int wrongReturn() { long a;
  return a;
}
typedef struct {
  int c;
  int b;
  int a;
  int d;
  int x[10];
} intStruct;

typedef struct {
  char c;
  char b;
  char a;
  char d;
  int x[10];
} myStruct;

myStruct returnStruct() {
  myStruct x;
  return x;
}

/*void variadicFunction(int n, ...) {}
void doVariadicCall() { variadicFunction(4, 3, 2, 1, 5); }
*/
void doCall() { doNothing(); }
void defaultIntPromotion(char x) {}
void defaultFloatPromotion(x) float x;
{}

struct big {
  char c[2047];
};
struct big functionWithBigStruct(struct big b) { return b; }
struct tiny {
  char c;
};
struct tiny functionWithTinyStruct(struct tiny e) { return e; }

extern int extFunc(intStruct strct);

static void func() {
  intStruct strt;
  intStruct *strtPtr;
  int x=23;
  strt.x[0] = 2;
  strtPtr = &strt;
  strtPtr->c = 5;
  strtPtr->b = 5;
  x = extFunc(*strtPtr);
  extFunc(*strtPtr);
}

typedef void FType(char a, char b);
extern FType *myFType;
extern void ftype(char a, char b);
typedef struct Person Person;
extern Person person;
extern void doPerson(FType *ftype, Person *person);
extern int __admStdIntrinsic_SomRandomIntrinsic(int a, int b);

unsigned int addSeven(char a, int s, short i, float f, myStruct strc,
                      char *iptr);
extern int extInt;
extern myStruct extGetStruct(int a, int b);
extern myStruct extStruct;
char getNextChar(int i);
static int myStaticInt = 17;
static int bssInt1;
static char bssChar1;
static float bssFloat1;
extern short getShort();
extern void useShort(short s);
extern char *cPtr;

static struct {
  char a;
  int b;
} someStruct = {'a', 15};
static const char a_char_literal[] = "hello world";

void ignoreReturnedStruct() { extGetStruct(1, 2); }

float constantFloat() { return 23.5; }

int useArgB(myStruct s) { return s.a; }
int *intPointer;
static int aFunction(float f1, float f2, unsigned int t) {
  myStruct strct = {5, 4, 3, 'f'};
  myStruct *strctPtr;
  float f3;
  double dbl;
  int x;
  unsigned int (*ptrToAddSeven)(char a, int s, short i, float f, myStruct strc,
                                char *iptr);
  signed char c;
  x = 57;

  c = __admStdIntrinsic_SomRandomIntrinsic(*intPointer, 115);

  useShort(*cPtr);
  doPerson(&ftype, &person);
  myFType('a', 'b');
  c = getNextChar(getShort());
  strct = extGetStruct(1, 2);
  x = bssFloat1;
  x = x + 1;
  getNextChar(x);
  f3 = bssInt1;
  f3 *= f3;
  strct.c = c;
  extStruct = strct;
  extStruct.b = 'a';
  strctPtr = &strct;
  strctPtr->b = myStaticInt;
  dbl = 37.0;
  f3 += f1 + f2;
  x = addSeven(11, 12, 13, f3, strct, &(strct.a));
  ptrToAddSeven = addSeven;
  x = (*ptrToAddSeven)(11, 12, 13, f3, *strctPtr, (void *)0);
  x += extInt;
  if (t == 87)
    return extInt;
  else
    return x;
}

unsigned int addSeven(char a, int s, short i, float f, myStruct strc,
                      char *iptr) {
  char x;
  x = 3 + a + i;
  return x;
}

void add9ToY(int t, int q) {
  int *p;
  addrOfY = &y;
  addr2OfY = &y;
}

int simpleCallingFunc(int x, int y, int z) {
  add9ToY(17, 19);
  return z + 3;
}

int doReturn() {
  int x;
  x = 7;
  return x;
}

void unsignedShiftRight() {
  unsigned int i = 7;
  i = i >> 1;
}

void branch() {
  int x = 1;
  if (x) {
    y = x;
  } else {
    y = x + 1;
  }
}

void callee();
void caller(void) { callee("wrong arg"); }
void callee() {}

void copyBytes(char *src, char *dst, int cnt) {
  for (; cnt>0; cnt--) {
    *dst++ = *src++;
  }
}

void whatIsFirstArg(int a, ...) {
  int i;
  i = __typecode(__firstarg) == 1;

}

long long addI8(long long a, long long b) { return a+b; }

/*
char
short
int
long
long_long
float
double
unsigned_char
signed_char
unsigned_short
unsigned_int
unsigned_long
unsigned_long_long
void_star_ptr
*/
typedef  unsigned char unsigned_char;
typedef  signed char signed_char;
typedef  unsigned short unsigned_short;
typedef  unsigned int unsigned_int;
typedef  unsigned long unsigned_long;
typedef long long long_long;
typedef unsigned long long unsigned_long_long;
typedef void *void_star_ptr;

#define CNV(a, b) \
  static b cnv_##a##_to_##b(a x) { return (b)x; }

CNV(char, char)
CNV(char, short)
CNV(char, int)
CNV(char, long)
CNV(char, long_long)
CNV(char, float)
CNV(char, double)
CNV(char, unsigned_char)
CNV(char, signed_char)
CNV(char, unsigned_short)
CNV(char, unsigned_int)
CNV(char, unsigned_long)
CNV(char, unsigned_long_long)
CNV(char, void_star_ptr)
CNV(short, char)
CNV(short, short)
CNV(short, int)
CNV(short, long)
CNV(short, long_long)
CNV(short, float)
CNV(short, double)
CNV(short, unsigned_char)
CNV(short, signed_char)
CNV(short, unsigned_short)
CNV(short, unsigned_int)
CNV(short, unsigned_long)
CNV(short, unsigned_long_long)
CNV(short, void_star_ptr)
CNV(int, char)
CNV(int, short)
CNV(int, int)
CNV(int, long)
CNV(int, long_long)
CNV(int, float)
CNV(int, double)
CNV(int, unsigned_char)
CNV(int, signed_char)
CNV(int, unsigned_short)
CNV(int, unsigned_int)
CNV(int, unsigned_long)
CNV(int, unsigned_long_long)
CNV(int, void_star_ptr)
CNV(long, char)
CNV(long, short)
CNV(long, int)
CNV(long, long)
CNV(long, long_long)
CNV(long, float)
CNV(long, double)
CNV(long, unsigned_char)
CNV(long, signed_char)
CNV(long, unsigned_short)
CNV(long, unsigned_int)
CNV(long, unsigned_long)
CNV(long, unsigned_long_long)
CNV(long, void_star_ptr)
CNV(long_long, char)
CNV(long_long, short)
CNV(long_long, int)
CNV(long_long, long)
CNV(long_long, long_long)
CNV(long_long, float)
CNV(long_long, double)
CNV(long_long, unsigned_char)
CNV(long_long, signed_char)
CNV(long_long, unsigned_short)
CNV(long_long, unsigned_int)
CNV(long_long, unsigned_long)
CNV(long_long, unsigned_long_long)
CNV(long_long, void_star_ptr)
CNV(float, char)
CNV(float, short)
CNV(float, int)
CNV(float, long)
CNV(float, long_long)
CNV(float, float)
CNV(float, double)
CNV(float, unsigned_char)
CNV(float, signed_char)
CNV(float, unsigned_short)
CNV(float, unsigned_int)
CNV(float, unsigned_long)
CNV(float, unsigned_long_long)
CNV(double, char)
CNV(double, short)
CNV(double, int)
CNV(double, long)
CNV(double, long_long)
CNV(double, float)
CNV(double, double)
CNV(double, unsigned_char)
CNV(double, signed_char)
CNV(double, unsigned_short)
CNV(double, unsigned_int)
CNV(double, unsigned_long)
CNV(double, unsigned_long_long)
CNV(unsigned_char, char)
CNV(unsigned_char, short)
CNV(unsigned_char, int)
CNV(unsigned_char, long)
CNV(unsigned_char, long_long)
CNV(unsigned_char, float)
CNV(unsigned_char, double)
CNV(unsigned_char, unsigned_char)
CNV(unsigned_char, signed_char)
CNV(unsigned_char, unsigned_short)
CNV(unsigned_char, unsigned_int)
CNV(unsigned_char, unsigned_long)
CNV(unsigned_char, unsigned_long_long)
CNV(unsigned_char, void_star_ptr)
CNV(signed_char, char)
CNV(signed_char, short)
CNV(signed_char, int)
CNV(signed_char, long)
CNV(signed_char, long_long)
CNV(signed_char, float)
CNV(signed_char, double)
CNV(signed_char, unsigned_char)
CNV(signed_char, signed_char)
CNV(signed_char, unsigned_short)
CNV(signed_char, unsigned_int)
CNV(signed_char, unsigned_long)
CNV(signed_char, unsigned_long_long)
CNV(signed_char, void_star_ptr)
CNV(unsigned_short, char)
CNV(unsigned_short, short)
CNV(unsigned_short, int)
CNV(unsigned_short, long)
CNV(unsigned_short, long_long)
CNV(unsigned_short, float)
CNV(unsigned_short, double)
CNV(unsigned_short, unsigned_char)
CNV(unsigned_short, signed_char)
CNV(unsigned_short, unsigned_short)
CNV(unsigned_short, unsigned_int)
CNV(unsigned_short, unsigned_long)
CNV(unsigned_short, unsigned_long_long)
CNV(unsigned_short, void_star_ptr)
CNV(unsigned_int, char)
CNV(unsigned_int, short)
CNV(unsigned_int, int)
CNV(unsigned_int, long)
CNV(unsigned_int, long_long)
CNV(unsigned_int, float)
CNV(unsigned_int, double)
CNV(unsigned_int, unsigned_char)
CNV(unsigned_int, signed_char)
CNV(unsigned_int, unsigned_short)
CNV(unsigned_int, unsigned_int)
CNV(unsigned_int, unsigned_long)
CNV(unsigned_int, unsigned_long_long)
CNV(unsigned_int, void_star_ptr)
CNV(unsigned_long, char)
CNV(unsigned_long, short)
CNV(unsigned_long, int)
CNV(unsigned_long, long)
CNV(unsigned_long, long_long)
CNV(unsigned_long, float)
CNV(unsigned_long, double)
CNV(unsigned_long, unsigned_char)
CNV(unsigned_long, signed_char)
CNV(unsigned_long, unsigned_short)
CNV(unsigned_long, unsigned_int)
CNV(unsigned_long, unsigned_long)
CNV(unsigned_long, unsigned_long_long)
#if 0
CNV(unsigned_long, void_star_ptr)
#endif
CNV(unsigned_long_long, char)
CNV(unsigned_long_long, short)
CNV(unsigned_long_long, int)
CNV(unsigned_long_long, long)
CNV(unsigned_long_long, long_long)
CNV(unsigned_long_long, float)
CNV(unsigned_long_long, double)
CNV(unsigned_long_long, unsigned_char)
CNV(unsigned_long_long, signed_char)
CNV(unsigned_long_long, unsigned_short)
CNV(unsigned_long_long, unsigned_int)
CNV(unsigned_long_long, unsigned_long)
CNV(unsigned_long_long, unsigned_long_long)
#if 0
CNV(unsigned_long_long, void_star_ptr)
#endif
#if 0
CNV(void_star_ptr, char)
CNV(void_star_ptr, short)
CNV(void_star_ptr, int)
CNV(void_star_ptr, long)
CNV(void_star_ptr, long_long)
CNV(void_star_ptr, unsigned_char)
CNV(void_star_ptr, signed_char)
CNV(void_star_ptr, unsigned_short)
CNV(void_star_ptr, unsigned_int)
CNV(void_star_ptr, unsigned_long)
CNV(void_star_ptr, unsigned_long_long)
CNV(void_star_ptr, void_star_ptr)
#endif

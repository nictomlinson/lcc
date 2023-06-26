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
unsigned long conU4_FF = 0 - 1;
float conF4_v1 = 12345.56789e5;
double conf8_v2 = 12345.56789e5;

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

void doNothing() {}
void (*doNothingFptr)(void) = doNothing;
void callThroughFunctionPointer() { doNothingFptr(); }

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
void floatToUnsignedShort(float f) { unsignedShortFromFloat = f; }
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

void allConversions() {
  char c;
  unsigned char uc;
  signed char sc;
  short s;
  unsigned short us;
  int i;
  unsigned int ui;
  long l;
  unsigned long ul;
  float f;
  double d;
  void *p;

  c = c;
  c = uc;
  c = sc;
  c = s;
  c = us;
  c = i;
  c = ui;
  c = l;
  c = ul;
  c = f;
  c = d;
  /*c = (char)p;*/

  uc = c;
  uc = uc;
  uc = sc;
  uc = s;
  uc = us;
  uc = i;
  uc = ui;
  uc = l;
  uc = ul;
  uc = f;
  uc = d;
  /*uc = (unsigned char)p;*/

  sc = c;
  sc = uc;
  sc = sc;
  sc = s;
  sc = us;
  sc = i;
  sc = ui;
  sc = l;
  sc = ul;
  sc = f;
  sc = d;
  /*sc = (signed char)p;*/

  s = c;
  s = uc;
  s = sc;
  s = s;
  s = us;
  s = i;
  s = ui;
  s = l;
  s = ul;
  s = f;
  s = d;
  /*s = (short)p;*/

  us = c;
  us = uc;
  us = sc;
  us = s;
  us = us;
  us = i;
  us = ui;
  us = l;
  us = ul;
#if (0)
  us = f;
  us = d;
#endif
  /*us = (unsigned short)p;*/

  i = c;
  i = uc;
  i = sc;
  i = s;
  i = us;
  i = i;
  i = ui;
  i = l;
  i = ul;
  i = f;
  i = d;
#if (0)
  i = (int)p;
#endif

  ui = c;
  ui = uc;
  ui = sc;
  ui = s;
  ui = us;
  ui = i;
  ui = ui;
  ui = l;
  ui = ul;
  ui = f;
  ui = d;
#if (0)
  ui = (unsigned int)p;
#endif

  l = c;
  l = uc;
  l = sc;
  l = s;
  l = us;
  l = i;
  l = ui;
  l = l;
  l = ul;
  l = f;
  l = d;
#if (0)
  l = (long)p;
#endif

  ul = c;
  ul = uc;
  ul = sc;
  ul = s;
  ul = us;
  ul = i;
  ul = ui;
  ul = l;
  ul = ul;
  ul = f;
  ul = d;
#if (0)
  ul = (unsigned long)p;
#endif

  f = c;
  f = uc;
  f = sc;
  f = s;
  f = us;
  f = i;
  f = ui;
  f = l;
  f = ul;
  f = f;
  f = d;
  /*f = (float)p;*/

  d = c;
  d = uc;
  d = sc;
  d = s;
  d = us;
  d = i;
  d = ui;
  d = l;
  d = ul;
  d = f;
  d = d;
  /*d = (double)p;*/

  p = (void *)c;
  p = (void *)uc;
  p = (void *)sc;
  p = (void *)s;
  p = (void *)us;
  p = (void *)i;
  p = (void *)ui;
  p = (void *)l;
#if (0) /*ignore this as it generates an error for magic1*/
  p = (void *)ul;
#endif
  /*p = (void *)f;*/
  /*p = (void *)d;*/
  p = (void *)p;
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

void variadicFunction(int n, ...) {}
void doCall() { doNothing(); }
void doVariadicCall() { variadicFunction(4, 3, 2, 1, 5); }
void defaultIntPromotion(char x) {}
void defaultFloatPromotion(x) float x;
{}

extern int extFunc(intStruct strct);

static void func() {
  intStruct strt;
  intStruct *strtPtr;
  int x;
  strt.x[0] = 2;
  strtPtr = &strt;
  strtPtr->c = 5;
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

static int aFunction(float f1, float f2, unsigned int t) {
  myStruct strct = {5, 4, 3, 'f'};
  myStruct *strctPtr;
  float f3;
  double dbl;
  int x;
  unsigned int (*ptrToAddSeven)(char a, int s, short i, float f, myStruct strc,
                                char *iptr);
  signed char c;

  c = __admStdIntrinsic_SomRandomIntrinsic(15, 115);

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
  add9ToY(y, x);
  return z + 3;
}

int doReturn() {
  int x;
  x = 7;
  return x;
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

extern unsigned char c1;
extern unsigned short c2;
extern char getChar();
extern short getShort();
extern char (*getChar_f)();
extern int getInt();
extern int i1;
typedef struct {
  char c;
} byte;
int adder() { return (*getChar_f)(); }
/*byte adder() {
  byte b;
  b.c = 'a';
  return b;
}*/

/*
    short c;
    short b;
    int a;
    char d;
*/

/*
extern int extFunc(myStruct strt);

static void func()
{
    myStruct strt;
    myStruct *strtPtr;
    strtPtr = &strt;
    strtPtr->c = 5;
    extFunc(*strtPtr);
}
*/

typedef struct {
  char c;
  char b;
  char a;
  char d;
} myStruct;

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

int doReturn() { return 117; }
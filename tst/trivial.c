typedef struct {
    short c;
    short b;
    int a;
    char d;
} myStruct;
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

static unsigned int addSeven(char a, int s, short i, float f, myStruct strc, int *iptr);
extern int extInt;


static int
aFunction(float f1, float f2, unsigned int t)
{
    myStruct strct;
    myStruct *strctPtr;
    float f3;
    double dbl;
    int x;
    unsigned int (*ptrToAddSeven)(char a, int s, short i, float f, myStruct strc, int *iptr);
    f3 = 54.3;
    strct.c = 53;
    strctPtr = &strct;
    strctPtr->b = 5;
    dbl = 37.0;
    f3 += f1 + f2;
    x = addSeven(11, 12, 13, f3, strct, &(strct.a));

    ptrToAddSeven = addSeven;
    x = (*ptrToAddSeven)(11, 12, 13, f3, *strctPtr, (void *)0);
    x += extInt;
    if(t == -87)
        return extInt;
    else
        return x;
}

static unsigned int addSeven(char a, int s, short i, float f, myStruct strc, int *iptr)
{
    return a;
}

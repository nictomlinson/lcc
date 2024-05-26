#ifndef SPLIT16_MD_PRE
#define SPLIT16_MD_PRE
#include "c.h"
#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)
#define relink(a, b) ((b)->x.prev = (a), (a)->x.next = (b))
#define DATA_STACK_CELL_CNT_LIMIT (((1<<(19-8))))

#define I(f) split16_##f

static char rcsid[] = "$Id$";

static int verbose = 0;
static int indent;
static int cseg = 0;  // current segment
static int dumpPacked = 0;
static int hostLittle; // set true if host is little endian'
static int genCost(int spaceCost, int timeCost);
static int cvSrcSize(Node a);  // size of the source value of a CVxxx node
static int isSeg(Node a, int seg); // true if ADDRGP symbol is in the segment
static int isFunction(Node a);
static int isNotFunction(Node a);
static int isAsmCall(Node a);
static int ifCost(int cond, int spaceCost, int timeCost);
static int inRange(Node a, int lo, int hi);

static void doarg(Node);

enum { POP = 45 << 4, ARGSTART = 46 <<4 }; // one more than VREG but not sure why VREG is 44
#define IS_EVEN(v) (((v)&1)==0)
static char *suffixes[] = {"0", "F", "D",  "C",  "S",  "I",  "U",  "P",
                           "V", "B", "10", "11", "12", "13", "14", "15"};
#endif

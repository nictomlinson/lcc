#ifndef __LCC__
#define __asmcall
#endif

static int __asmcall f(void) { return 0; }

static int g(void) { return f(); }
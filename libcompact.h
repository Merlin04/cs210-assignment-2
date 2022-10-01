#include <stdint.h>

// Some nice shorthand
#define R return
typedef char C;typedef int I;typedef void V;typedef V(*F)(); // F is function pointer
typedef uint64_t U64;typedef uint32_t U32;typedef uint16_t U16;typedef uint8_t U8;typedef int64_t I64;typedef int32_t I32;typedef int16_t I16;typedef int8_t I8;
#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX
#define U32_MAX UINT32_MAX
#define U64_MAX UINT64_MAX
#define I8_MAX INT8_MAX
#define I16_MAX INT16_MAX
#define I32_MAX INT32_MAX
#define I64_MAX INT64_MAX

// general utils
#define P printf
#define Pn(s) P("%s\n",s) // print string with newline
#define Pd(d) P("%d\n",d) // print digit with newline
#define Pf(f) P("%f\n",f)
#define Pb(b) printb(b) // print binary number with newline
#define Pt(t) P("{%d, %d}\n",t[0],t[1]) // print ordered pair
#define _ , // used to provide a comma inside a macro argument (otherwise it will think it is multiple arguments)
#define ESC_(...) __VA_ARGS__
#define ESC(...) ESC_(__VA_ARGS__) // allow commas
#define DO(n,x) {I i=0,_n=(n);for(;i<_n;++i){x;}} // compact loop

// utils for testing
#define VA_N_(a,b,c,d,e,f,g,h,i,j,N,...) N // helper for va counter, args except for N are not used
#define VA_N(...) VA_N_(__VA_ARGS__ __VA_OPT__(,) 10,9,8,7,6,5,4,3,2,1,0) // count number of variadic arguments, using a trick that involves offsetting a list of descending numbers by the va args then getting the number at where the end of the number list initially was
#define DOA(T,x,...) {I n=VA_N(__VA_ARGS__);T a[VA_N(__VA_ARGS__)]={__VA_ARGS__};DO(n,x)} // for-each loop (T=type, x=fn)
#define T(f,t,P,...) DOA(t,P(f(a[i])),__VA_ARGS__) // Run/print fn result for each test value
#define Tl(f,t,P,l,...) {t r[l]=__VA_ARGS__;DO(l,P(f(r[i])))} // Allow passing array literal with length, this is useful because it means you can include commas in the individual arguments since it doesn't rely on VA_N
#define CAT(A,B) A ## B // concat string
#define Tf(i,f,y,P,...) V CAT(t,i)(){T(f,y,P,__VA_ARGS__)} // declare test function for given values (i=problem number, f=fn, y=type of args, P=print fn
#define Tfl(i,f,y,P,l,...) V CAT(t,i)(){Tl(f,y,P,l,__VA_ARGS__)}
#define Tr(...) DOA(F,P("---Question ");Pd(i+1);a[i](),__VA_ARGS__) // run each function given with header
#define Ts(i,f,RT,s,...) typedef struct {s} CAT(args,i);RT CAT(w,i)(CAT(args,i) a){R f(__VA_ARGS__);} // define wrapper function that accepts a struct with arguments (i=problem number, f=fn, RT=return type, s=struct def, ...=manual spread of struct props into arguments)
#define DEPAREN_(...) __VA_ARGS__
#define DEPAREN(a) DEPAREN_ a
#define Tfs(i,f,P,RT,s,p,l,...) Ts(i,f,RT,s,DEPAREN(p)) V CAT(t,i)(){Tl(CAT(w,i),CAT(args,i),P,l,__VA_ARGS__)} // declare test function with struct for args (i=problem number, f=fn, P=print fn, RT=return type, s=struct def, p=manual spread of struct props into arguments, l=length)
#define MAKE_TEST_FN_STRUCT Tfs
#define RUN_FNS Tr
#define MAKE_TEST_FN Tf

V printb(U32 i);
C* int2str(long int val,C* dst,I radix,I upcase);

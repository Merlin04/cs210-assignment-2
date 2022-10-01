#include "libcompact.h"
#include <stdlib.h>
#include <stdio.h>

V printb(U32 i){char b[33];int2str(i,b,2,0);printf("0b%s\n",b);}


// https://github.com/mysql/mysql-server/blob/5.7/strings/int2str.c
char _dig_vec_upper[] =
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char _dig_vec_lower[] =
  "0123456789abcdefghijklmnopqrstuvwxyz";

#define NullS NULL
typedef unsigned char uchar;

char *
int2str(long int val, char *dst, int radix, 
        int upcase)
{
  char buffer[65];
  char *p;
  long int new_val;
  char *dig_vec= upcase ? _dig_vec_upper : _dig_vec_lower;
  ulong uval= (ulong) val;

  if (radix < 0)
  {
    if (radix < -36 || radix > -2)
      return NullS;
    if (val < 0)
    {
      *dst++ = '-';
      /* Avoid integer overflow in (-val) for LLONG_MIN (BUG#31799). */
      uval = (ulong)0 - uval;
    }
    radix = -radix;
  }
  else if (radix > 36 || radix < 2)
    return NullS;

  /*
    The slightly contorted code which follows is due to the fact that
    few machines directly support unsigned long / and %.  Certainly
    the VAX C compiler generates a subroutine call.  In the interests
    of efficiency (hollow laugh) I let this happen for the first digit
    only; after that "val" will be in range so that signed integer
    division will do.  Sorry 'bout that.  CHECK THE CODE PRODUCED BY
    YOUR C COMPILER.  The first % and / should be unsigned, the second
    % and / signed, but C compilers tend to be extraordinarily
    sensitive to minor details of style.  This works on a VAX, that's
    all I claim for it.
  */
  p = &buffer[sizeof(buffer)-1];
  *p = '\0';
  new_val= uval / (ulong) radix;
  *--p = dig_vec[(uchar) (uval- (ulong) new_val*(ulong) radix)];
  val = new_val;
  while (val != 0)
  {
    ldiv_t res;
    res=ldiv(val,radix);
    *--p = dig_vec[res.rem];
    val= res.quot;
  }
  while ((*dst++ = *p++) != 0) ;
  return dst-1;
}

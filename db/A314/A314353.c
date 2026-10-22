/*
 * A314353  Coordination sequence Gal.6.567.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A314353: Coordination sequence Gal.6.567.2
   a(0) = 1
   For n >= 1 the value can be written as a(n) = 5*n + d(n)
   where d(n) depends only on n mod 4 and k = floor(n/4):
   - if r = 0 (n % 4 == 0)        : d = 0
   - if r = 1 :
       * if k is even  (k%2==0)   : d = –2*k
       * if k is odd   (k%2==1)   : d = +1
   - if r = 2 :
       * if k is even  : d = –(2*k+3)
       * if k is odd   : d = –(2*k–1)
   - if r = 3 :
       * if k is even  : d = –1
       * if k is odd   : d = –2*(k+1)
   This formula reproduces all terms of the sequence.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* special case */
        return 1;

    idx_t k = n / 4;      /* floor division */
    idx_t r = n % 4;
    int64_t diff = 0;     /* adjustment term d(n) */

    switch (r)
    {
        case 0:
            diff = 0;
            break;
        case 1:
            if (k & 1)          /* k odd */
                diff = 1;
            else                /* k even */
                diff = - (int64_t)(2 * k);
            break;
        case 2:
            if (k & 1)          /* k odd */
                diff = - ((int64_t)2 * k - 1);
            else                /* k even */
                diff = - ((int64_t)2 * k + 3);
            break;
        case 3:
            if (k & 1)          /* k odd */
                diff = - ((int64_t)2 * (k + 1));
            else                /* k even */
                diff = -1;
            break;
        default:                /* unreachable */
            return 0;
    }

    seq_t base = (seq_t)n * 5;     /* 5*n in the target precision */

    if (diff >= 0)
        return base + (seq_t)diff;
    else
        return base - (seq_t)(-diff);    /* diff is negative */
}

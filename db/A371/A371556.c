/*
 * A371556  Table read by rows: row n is the unique primitive Pythagorean
 *          quadruple (a,b,c,d) such that (a + b + c - d)/2 = 2^n - 1 and a =
 *          b = 2^n.
 * https://oeis.org/A371556
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A371556 – inradius elements of primitive Pythagorean quadruples
   Formula: for n >= 2, let r = 2 + floor((n-2)/4);
   then a(n) = 2^r if (n-2) mod 4 = 0 or 1
              2^(2r-1) - 1 if (n-2) mod 4 = 2
              2^(2r-1) + 1 if (n-2) mod 4 = 3
*/

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* offset not used, but safe */
    idx_t m   = n - 2;                  /* offset from first term */
    idx_t r   = 2 + m / 4;              /* row index */
    idx_t pos = m % 4;                  /* position in the row: 0=a,1=b,2=c,3=d */

    if (pos == 0 || pos == 1) {         /* a or b = 2^r */
        return pow2(r);
    } else {
        seq_t p = pow2(2 * r - 1);      /* 2^(2r-1) */
        return (pos == 2) ? (p - 1) : (p + 1);   /* c or d */
    }
}

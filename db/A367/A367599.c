/*
 * A367599  Indices of record high-points in A367356.
 * https://oeis.org/A367599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367599: indices of record high-points in A367356.
   Formula:
     a(1) = 1
     a(2) = 6
     a(3) = 7
     a(n) = 6 * 3^(2n-7)   for all n ≥ 4
   (3^k computed via pow3(k), seq_t is arbitrary-precision integer).
*/
#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0U)          /* just in case; offset starts at 1 */
        return 0;
    else if (n == 1U)
        return (seq_t)1;
    else if (n == 2U)
        return (seq_t)6;
    else if (n == 3U)
        return (seq_t)7;
    else
    {
        /* exponent = 2n - 7, for n >= 4 this is non‑negative */
        idx_t exp = (2U * n) - 7U;
        seq_t val = pow3(exp);          /* 3^exp */
        val = val * (seq_t)6;           /* multiply by 6 */
        return val;
    }
}

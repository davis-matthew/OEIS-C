/*
 * A364811  Number of distinct residues x^4 (mod 2^n), x=0..2^n-1.
 * https://oeis.org/A364811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ceil(2^n / 15) + 1 - [n mod 4 == 0] (where [P] is 1 if
 *            the predicate P is true, 0 otherwise)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);          /* v = 2^n */
    v += 14;                    /* v = 2^n + 14 */
    v /= 15;                    /* v = ceil(2^n / 15) */
    v += 1;                     /* v = ceil(2^n / 15) + 1 */
    if ((n & 3) == 0)           /* n mod 4 == 0 */
        v -= 1;                 /* v = ceil(2^n / 15) + 1 - 1 */
    return v;
}

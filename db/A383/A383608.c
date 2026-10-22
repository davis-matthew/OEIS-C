/*
 * A383608  Expansion of e.g.f. (1+x)*cosh(x)^2.
 * https://oeis.org/A383608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(n)=2^(n‑1) for even n>0 a(n)=n*2^(n‑2) for odd
 *            n>1 (1+x)*cosh(x)^2 expansion
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)      /* base cases */
        return 1;

    if ((n & 1) == 0) {        /* even n>0 */
        return pow2(n - 1);
    } else {                   /* odd n>1 */
        return (seq_t)n * pow2(n - 2);
    }
}

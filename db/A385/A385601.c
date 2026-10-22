/*
 * A385601  Expansion of e.g.f. cosh(x)^2*(x+x^2/2).
 * https://oeis.org/A385601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1; * a(n) = n * 2^(n-2) for odd n ≥ 3; *
 *            a(n) = binomial(n,2) * 2^(n-3) for even n ≥ 4; * Implements OEIS
 *            A385601
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n <= 2)
        return 1;

    if (n & 1) {                    /* odd n */
        /* a(n) = n * 2^(n-2) */
        return (seq_t)n * pow2(n - 2);
    } else {                        /* even n */
        /* a(n) = binomial(n,2) * 2^(n-3) = n*(n-1)/2 * 2^(n-3) */
        seq_t binom = ((seq_t)n * (seq_t)(n - 1)) / 2;
        return binom * pow2(n - 3);
    }
}

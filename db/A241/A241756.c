/*
 * A241756  A finite sum of products of binomial coefficients: Sum_(m=0..n)
 *          binomial(-1/4, m)^2*binomial(-1/4, n-m)^2 (C. C. Grosjean's
 *          problem, denominators).
 * https://oeis.org/A241756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = a(n‑1) * 8^(1+v₂(n)), a(0) = 1 * where v₂(n) is the
 *            exponent of 2 dividing n (number of trailing zeros in n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)  */
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

static inline idx_t v2(idx_t n)
{
    idx_t c = 0;
    while ((n & 1) == 0) { /* n is even */
        ++c;
        n >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* exponent increment for this step */
        idx_t exp = 1 + v2(i);
        /* factor = 8^exp */
        seq_t factor = seq_pow((seq_t)8, exp);
        /* multiply */
        result = seq_mul(result, factor);
    }

    return result;
}

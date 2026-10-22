/*
 * A321234  Denominator of series expansion of the hypergeometric series
 *          3F2([1/2, 1, 1], [3/2, 3/2], x).
 * https://oeis.org/A321234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(binomial(2*n, n) / 4^n) * (2*n + 1)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute binomial(2*n, n) */
    seq_t b = binom((idx_t)(2 * n), n);

    /* Count 1-bits in n to know how many factors of 2 are in the binomial */
    idx_t pc = (idx_t)__builtin_popcountll((unsigned long long)n);

    /* Divide out all powers of 2 from the binomial: b / 2^pc */
    seq_t num = b / pow2(pc);

    /* Multiply by (2*n + 1)^2 */
    seq_t den = (seq_t)(2 * n + 1);
    seq_t ans = num * den * den;

    return ans;
}

/*
 * A307522  Expansion of Product_{k>=1} ((1 + x)^k - x^k)/((1 + x)^k + x^k).
 * https://oeis.org/A307522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * (-1)^n * Σ_{k=1..⌊√n⌋} C(n-1, n-k²) for n≥1, a(0)=1
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* compute floor(sqrt(n)) by a small loop (n ≤ 10000, so fast enough) */
    idx_t limit = 0;
    while ((limit + 1) * (limit + 1) <= n)
        ++limit;

    seq_t sum = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        /* n - k^2 is between 0 and n-1 */
        idx_t m = n - k * k;
        sum += binomial(n - 1, m);
    }

    /* multiply by 2 */
    seq_t res = sum << 1;

    /* apply (-1)^n */
    if (n & 1)
        res = -res;

    return res;
}

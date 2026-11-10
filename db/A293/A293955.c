/*
 * A293955  Partial sums of A055612.
 * https://oeis.org/A293955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{k=0..n} b(k), where b(k) = Product_{m=1..k}
 *            (binomial(k,m)+1) (A055612), b(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 66 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A055612_a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 1;
    seq_t binom = 1;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t numerator = (seq_t)(n - m + 1);
        seq_t denominator = (seq_t)m;
        binom = binom * numerator / denominator;
        result = result * (binom + 1);
    }

    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
        sum += A055612_a(k);
    return sum;
}

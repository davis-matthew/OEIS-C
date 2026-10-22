/*
 * A392076  a(n) = Sum_{k=0..n} (k+1) * binomial(k,4*(n-k)).
 * https://oeis.org/A392076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=0..n} (k+1) * binomial(k, 4*(n-k))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 67, u128 n <= 137,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    // Start of summation: ceil(4*n / 5)
    idx_t start = (4 * n + 4) / 5;   // integer arithmetic, 4*n fits in idx_t for all practical n

    for (idx_t k = start; k <= n; ++k) {
        idx_t m = 4 * (n - k);          // 4*(n-k) is always <= k due to start selection
        seq_t bin = binomial(k, m);     // binomial(k, m) returns 0 if m > k
        seq_t term = ((seq_t)(k + 1)) * bin;
        sum += term;
    }

    return sum;
}

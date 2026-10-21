/*
 * A247415  Number of friezes of type D_n.
 * https://oeis.org/A247415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{m=1..n} d(m) * binomial(2*n-m-1, n-m) where d(m) is
 *            the number of divisors of m
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"  /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    static idx_t *div_counts = NULL;
    static size_t div_cap = 0;
    idx_t i, j;

    /* Allocate/reallocate storage for divisor counts up to n */
    if (n > div_cap) {
        free(div_counts);
        div_counts = (idx_t*)malloc((n + 1) * sizeof(idx_t));
        div_cap = (size_t)n;
    }

    /* Reset divisor counts */
    for (i = 1; i <= n; ++i)
        div_counts[i] = 0;

    /* Simple sieve to compute divisor counts d(m) for 1..n */
    for (i = 1; i <= n; ++i) {
        for (j = i; j <= n; j += i)
            ++div_counts[j];
    }

    /* Compute the sum using the formula */
    seq_t sum = 0;
    for (i = 1; i <= n; ++i) {
        seq_t bin = binomial(2 * n - i - 1, n - i);
        sum += (seq_t)div_counts[i] * bin;
    }

    return sum;
}

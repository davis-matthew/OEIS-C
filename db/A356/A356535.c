/*
 * A356535  a(n) = Sum_{k=1..n} sigma_2(k)^2.
 * https://oeis.org/A356535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} sigma_2(k)^2
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 107, u64 n <= 9105,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n).  This routine recomputes all sigma_2 values up
 * to n from scratch.  For n ≤ 10 000 the total work is
 * O(n log n) and comfortably fits the 2‑second limit. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, return 0 */

    /* Allocate an array for sigma_2 values (index 0 unused). */
    seq_t *sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma)
        abort();

    /* Initialize all entries to 0. */
    for (idx_t i = 0; i <= n; ++i)
        sigma[i] = 0;

    /* Sieve for sigma_2: for each i, add i² to all multiples of i. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_sq = (seq_t)i * (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += i_sq;
    }

    /* Accumulate the sum of squares. */
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq = sigma[i] * sigma[i];
        result += sq;
    }

    free(sigma);
    return result;
}

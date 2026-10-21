/*
 * A253248  Number of k <= n with A000203(k) <= A000203(n).
 * https://oeis.org/A253248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of k ≤ n with σ(k) ≤ σ(n) (σ = A000203)
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate array [0..n] for divisors sums, 0 unused */
    seq_t *sigma = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!sigma)
        return 0;   /* allocation failed; return 0 as placeholder */

    /* Initialize to zero */
    for (idx_t i = 0; i <= n; ++i)
        sigma[i] = 0;

    /* Sieve to compute σ(k) for all k ≤ n */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            sigma[j] += (seq_t)i;
    }

    seq_t sigma_n = sigma[n];
    seq_t count = 0;

    for (idx_t k = 1; k <= n; ++k)
        if (sigma[k] <= sigma_n)
            ++count;

    free(sigma);
    return count;
}

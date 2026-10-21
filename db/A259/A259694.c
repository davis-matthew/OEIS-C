/*
 * A259694  a(n) = Sum(k^6*sigma(k)*sigma(n-k),k=1..n-1).
 * https://oeis.org/A259694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(k^6*sigma(k)*sigma(n-k),k=1..n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 17, u64 n <= 197, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;               /* out-of-range guard */

    /* allocate array for divisor sums sigma[0..n-1] */
    seq_t *sigma = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    if (!sigma) return (seq_t)0;               /* allocation failure guard */

    /* initialise sigma */
    for (idx_t i = 0; i < n; ++i) sigma[i] = (seq_t)0;
    sigma[0] = (seq_t)0;                       /* unused, but defined */

    /* sieve to compute sigma(k) for 1 <= k < n */
    for (idx_t i = 1; i < n; ++i)
    {
        seq_t val = (seq_t)i;
        for (idx_t j = i; j < n; j += i)
            sigma[j] += val;
    }

    /* compute the convolution sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t kk   = (seq_t)k;
        seq_t k2   = kk * kk;          /* k^2   */
        seq_t k4   = k2 * k2;          /* k^4   */
        seq_t k6   = k4 * k2;          /* k^6   */
        seq_t term = k6 * sigma[k] * sigma[n - k];
        sum += term;
    }

    free(sigma);
    return sum;
}

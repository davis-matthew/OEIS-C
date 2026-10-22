/*
 * A367239  G.f. A(x) satisfies A(x) = 1 + x*A(x) / (1 - x*A(x)^2)^3.
 * https://oeis.org/A367239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binom(2*n - k, k-1) * binom(n + 2*k - 1, n -
 *            k) / k
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n < cache_len) return cache[n];

    /* Expand cache if needed */
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
    if (!cache) return 0;  /* allocation failure, return 0 */

    /* Compute values from old_len up to n */
    for (idx_t m = old_len; m <= n; ++m) {
        seq_t sum = 0;
        seq_t r = 1;  /* C(2*m - 1, 0) for k=1 */
        seq_t s = (seq_t)m * (m + 1) / 2;  /* C(m + 1, 2) for k=1 */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t prod = r * s;
            seq_t term = prod / (seq_t)k;
            sum += term;

            if (k < m) {
                /* Update r: C(2*m - (k+1), k) from C(2*m - k, k-1) */
                seq_t num_r = (seq_t)(2*m - 2*k + 1) * (seq_t)(2*m - 2*k);
                seq_t den_r = (seq_t)k * (seq_t)(2*m - k);
                r = r * num_r / den_r;

                /* Update s: C(m + 2*(k+1) - 1, m - (k+1)) from C(m + 2*k - 1, m - k) */
                seq_t num_s = (seq_t)(m + 2*k + 1) * (seq_t)(m + 2*k) * (seq_t)(m - k);
                seq_t den_s = (seq_t)(3*k + 2) * (seq_t)(3*k + 1) * (seq_t)(3*k);
                s = s * num_s / den_s;
            }
        }
        cache[m] = sum;
    }

    return cache[n];
}

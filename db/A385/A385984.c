/*
 * A385984  a(0) = 1; a(n) = Sum_{k=0..n-1} (-2)^k * binomial(n-1,k) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A385984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = Σ_{k=0}^{n-1} (-2)^k * binomial(n-1,k) * a(k) *
 *            a(n-1-k)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t length = 0;  /* number of entries currently stored */

    /* Grow cache if necessary */
    if (n >= cap) {
        idx_t newcap = (cap == 0) ? 1 : cap << 1;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* Ensure base case a(0)=1 */
    if (length == 0) {
        cache[0] = 1;
        length = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t m = length; m <= n; ++m) {
        if (m == 0) continue;   /* already known */

        seq_t sum  = 0;
        seq_t pow  = 1;          /* (-2)^k, start with k=0 */
        seq_t binom = 1;         /* binomial(m-1,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* term = (-2)^k * binomial(m-1,k) * a(k) * a(m-1-k) */
            seq_t term = pow * binom * cache[k] * cache[m - 1 - k];
            sum += term;

            /* update pow and binom for next k */
            pow = pow * (seq_t)(-2);
            if (k + 1 < m) {
                seq_t numerator   = (seq_t)(m - 1 - k);
                seq_t denominator = (seq_t)(k + 1);
                binom = (binom * numerator) / denominator;   /* exact division */
            }
        }
        cache[m] = sum;
    }
    length = n + 1;

    return cache[n];
}

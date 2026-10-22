/*
 * A302196  Hurwitz logarithm of triangular numbers [1,3,6,10,15,...].
 * https://oeis.org/A302196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = log₁(Σ[(n+1)(n+2)/2]·xⁿ/n!), a(0)=0 Recurrence (derived
 *            from exp₁ of the Hurwitz logarithm): a(0)=0 a(n)= T(n) –
 *            Σ_{k=1}^{n-1} C(n‑1,k‑1)·a(k)·T(n‑k) where T(m)=(m+1)(m+2)/2 and
 *            C are ordinary binomial coefficients
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;    /* cache of computed a(k) values */
    static idx_t cache_sz = 0;    /* allocated size */
    static idx_t cache_len = 0;   /* number of entries filled (max index+1) */

    /* ensure buffer large enough for n+1 entries */
    if (n >= cache_len) {
        /* grow buffer */
        size_t new_sz = (n + 1 > cache_sz * 2) ? (n + 1) : cache_sz * 2;
        if (new_sz <= cache_sz) new_sz = cache_sz + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) return 0; /* cannot allocate */
        cache = tmp;
        cache_sz = new_sz;

        /* fill missing entries */
        for (idx_t m = cache_len; m <= n; ++m) {
            if (m == 0) {          /* base case */
                cache[0] = 0;
                continue;
            }
            /* compute T(m) = (m+1)(m+2)/2 */
            seq_t Tm = (seq_t)(m + 1) * (seq_t)(m + 2) / 2;

            /* sum over k=1..m-1  of C(m-1,k-1) * a(k) * T(m-k) */
            seq_t sum = 0;
            seq_t binom = 1; /* C(m-1,0) */

            for (idx_t k = 1; k < m; ++k) {
                /* T(m-k) = (m-k+1)(m-k+2)/2 */
                seq_t Tk = (seq_t)(m - k + 1) * (seq_t)(m - k + 2) / 2;
                sum += binom * cache[k] * Tk;

                /* advance binomial coefficient: C(m-1,k) = C(m-1,k-1)*(m-k)/k */
                if (k < m - 1) {
                    binom = binom * (seq_t)(m - k) / (seq_t)k;
                }
            }
            cache[m] = Tm - sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
}

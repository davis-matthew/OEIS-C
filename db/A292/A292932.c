/*
 * A292932  Number of quasitrivial semigroups on an arbitrary n-element set.
 * https://oeis.org/A292932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(n+1) = (n+1)*a(n) + 2*Sum_{k=0}^{n-1}
 *            binomial(n+1,k)*a(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;

    /* extend cache if needed (allocate at least n+1 entries) */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 64) new_cap = 64;             /* small initial size */
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;            /* handle allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
        }
        /* zero initialise new slots */
        for (size_t i = cap; i < new_cap; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute terms up to a[n] if not already present */
    while (len <= n) {
        idx_t m = len;                /* compute a[m] */
        if (m == 0) {                 /* a(0) = 1 */
            cache[0] = 1;
            ++len;
            continue;
        }

        seq_t sum = 0;                /* Sum_{k=0}^{m-2} binomial(m,k)*a(k) */
        seq_t C   = 1;

        for (idx_t k = 0; k + 1 < m; ++k) {
            sum += C * cache[k];
            /* update C to binomial(m, k+1) */
            seq_t temp = C * ((seq_t)(m - k));
            C = temp / ((seq_t)(k + 1));
        }

        seq_t term1 = ((seq_t)m) * cache[m - 1];
        seq_t term2 = ((seq_t)2) * sum;
        cache[m] = term1 + term2;

        ++len;
    }

    return cache[n];
}

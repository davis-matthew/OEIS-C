/*
 * A306624  Expansion of e.g.f. exp(Sum_{k=1..9} x^k).
 * https://oeis.org/A306624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = (n-1)! * Σ_{k=1..min(9,n)} k * a(n-k) /
 *            (n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0]..a[max_n] */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;      /* allocated slots */
    static idx_t  max_n = 0;      /* highest computed index */

    /* ensure capacity for n+1 elements */
    if (n >= cap) {
        idx_t new_cap = (cap == 0 ? 1 : cap);
        while (new_cap <= n)
            new_cap <<= 1;
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache,
                                                        new_cap * sizeof(seq_t)));
        cache = tmp;
#else
        cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
        cap = new_cap;
    }

    /* initialise first term */
    cache[0] = (seq_t)1;

    /* compute missing values up to n */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = (i < 9 ? i : 9);
            for (idx_t k = 1; k <= kmax; ++k) {
                /* compute (i-1)! / (i-k)! = (i-1)*(i-2)*...*(i-k+1) */
                seq_t prod = 1;
                for (idx_t j = 1; j < k; ++j)
                    prod *= (seq_t)(i - j);
                /* term = k * a[i-k] * prod */
                seq_t term = (seq_t)k * cache[i - k] * prod;
                sum += term;
            }
            cache[i] = sum;
        }
        max_n = n;
    }

    return cache[n];
}

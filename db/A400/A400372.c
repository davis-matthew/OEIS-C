/*
 * A400372  Expansion of Product_{k>=0} (1 + Sum_{j>=0} x^(3^(k+j))).
 * https://oeis.org/A400372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(0)=1, a(1)=1, a(2)=0; a(3*n) = Σ_{k≥0, 3^k≤n} a(n-3^k);
 *            a(3*n+1) = a(n); a(3*n+2) = 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache for computed terms */
    static idx_t cache_size = 0;     /* allocated size of cache */
    static idx_t top_computed = 0;   /* highest index already computed */

    /* ensure space for indices 0 … n */
    size_t needed = (size_t)(n + 1 > 3 ? n + 1 : 3);
    if (cache == NULL) {
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache_size = (idx_t)needed;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        top_computed = 2;
    } else if ((idx_t)needed > cache_size) {
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache_size = (idx_t)needed;
    }

    if (n <= top_computed)
        return cache[n];

    for (idx_t i = top_computed + 1; i <= n; ++i) {
        if (i % 3 == 0) {
            idx_t m = i / 3;
            seq_t sum = cache[m];          /* a(m) */
            if (m >= 1)
                sum += cache[m - 1];       /* a(m-1) */
            for (idx_t pow = 3; pow <= m; pow *= 3) {
                sum += cache[m - pow];     /* a(m-3^k) */
            }
            cache[i] = sum;
        } else if (i % 3 == 1) {
            idx_t m = (i - 1) / 3;
            cache[i] = cache[m];
        } else { /* i % 3 == 2 */
            cache[i] = (seq_t)0;
        }
    }

    top_computed = n;
    return cache[n];
}

/*
 * A343688  a(1)=1, a(2)=0, a(n) = (4*n-2)*a(n-1) + a(n-2), n > 2.
 * https://oeis.org/A343688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=0, a(n) = (4*n-2)*a(n-1) + a(n-2) for n>2 (OEIS
 *            A343688)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization – 1‑based indexing */
    static seq_t *cache     = NULL;      /* holds a[i], 1 <= i <= max_comp */
    static idx_t  cache_cap = 0;         /* allocated capacity (>=1)   */
    static idx_t  max_comp  = 0;         /* highest index computed so far */

    /* enlarge cache if necessary (always leave index 0 unused) */
    if (cache_cap < n+1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < n+1) new_cap = n+1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two terms on first use */
    if (max_comp < 1) {
        cache[1] = (seq_t)1;
        max_comp = 1;
    }
    if (max_comp < 2) {
        cache[2] = (seq_t)0;
        if (max_comp < 1) cache[1] = (seq_t)1;
        max_comp = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t coeff = (seq_t)((seq_t)4 * (seq_t)i - (seq_t)2);
        seq_t val   = coeff * cache[i-1] + cache[i-2];
        cache[i]   = val;
    }
    max_comp = (max_comp < n) ? n : max_comp;

    return cache[n];
}

/*
 * A308939  Expansion of e.g.f. 1 / (1 - Sum_{k>=1} (2*k - 1)!!*x^k/k!).
 * https://oeis.org/A308939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5. For n>=3: a(n) = ((14*n-9)*a(n-1) -
 *            8*(n-1)*(2*n-3)*a(n-2)) / 3 (This recurrence follows the OEIS
 *            D‑finite relation.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;           /* highest index computed */

    /* initial allocation: 16 elements (will grow as needed) */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        /* initialise known terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache_last = 2;
    } else if ((size_t)n >= cache_cap) {
        /* grow cache to at least n+1 */
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if requested value already computed, return it */
    if (n <= cache_last) return cache[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 0) {                 /* should not happen, guard */
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)5;
        } else {
            /* term1 = (14*i - 9) * a(i-1) */
            seq_t term1 = ((seq_t)((seq_t)14 * (seq_t)i - (seq_t)9)) * cache[i-1];
            /* term2 = 8*(i-1)*(2*i-3) * a(i-2) */
            seq_t term2 = ((seq_t)8 * (seq_t)(i-1) * (seq_t)((seq_t)2 * (seq_t)i - (seq_t)3)) * cache[i-2];
            cache[i] = (term1 - term2) / (seq_t)3;
        }
    }
    cache_last = n;
    return cache[n];
}

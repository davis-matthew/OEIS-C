/*
 * A383316  Expansion of e.g.f. exp(x/2) / (1-4*x)^(1/8).
 * https://oeis.org/A383316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n-3)*a(n-1) - 2*(n-1)*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t len = 0;

    /* ensure space for the first two elements */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* if value already computed, return it */
    if (n < len)
        return cache[n];

    /* grow cache if needed */
    while (len <= n) {
        if (len >= cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                free(cache);
                return 0; /* allocation failed */
            }
            cache = new_cache;
            cap = new_cap;
        }
        /* compute next term using recurrence */
        idx_t i = len;
        seq_t term1 = ((seq_t)(4 * i - 3)) * cache[i - 1];
        seq_t term2 = ((seq_t)(2 * (i - 1))) * cache[i - 2];
        cache[i] = term1 - term2;
        len++;
    }
    return cache[n];
}

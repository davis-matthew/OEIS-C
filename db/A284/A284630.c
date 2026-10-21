/*
 * A284630  a(1)=1, a(2)=2; for n > 1, a(n+1) = (a(n-1) mod n) + n.
 * https://oeis.org/A284630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=1, a(2)=2; for n>1, a(n+1) = (a(n-1) mod n) + n
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* 1‑based array: a_cache[1] = 1, a_cache[2] = 2 */
static size_t a_cache_cap = 0;  /* allocated capacity (largest index that may be stored) */
static idx_t   a_cache_size = 0;/* highest index that has a valid value */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined region – return 0 */

    /* initialise cache with base values if it does not exist yet */
    if (a_cache == NULL) {
        a_cache_cap = 16;
        a_cache = (seq_t *)malloc((a_cache_cap + 1) * sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)2;
        a_cache_size = 2;
    }

    /* grow the cache if necessary */
    if ((size_t)n > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    if (n <= a_cache_size)
        return a_cache[n];

    /* compute a(n) and all intermediate terms up to n */
    for (idx_t i = a_cache_size + 1; i <= n; ++i) {
        seq_t prev = a_cache[i - 2];                       /* a(i-2) */
        seq_t mod  = prev % ((seq_t)(i - 1));              /* a(i-2) mod (i-1) */
        a_cache[i] = mod + ((seq_t)(i - 1));               /* a(i) */
    }
    a_cache_size = n;

    return a_cache[n];
}

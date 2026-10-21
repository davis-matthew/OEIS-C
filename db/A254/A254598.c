/*
 * A254598  Numbers of n-length words on alphabet {0,1,...,8} with no subwords
 *          ii, for i from {0,1}.
 * https://oeis.org/A254598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 7*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation of all already computed terms */
    static seq_t *cache = NULL;   /* array of terms, cache[i] = a(i) */
    static idx_t cache_size = 0;  /* number of terms stored (>= cache_size) */

    /* handle the first two terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    /* initial allocation for the first two terms if not done yet */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }

    /* if n exceeds what we have cached, extend the cache */
    if (n >= cache_size) {
        /* new capacity needed */
        idx_t new_cap = n + 1;              /* store up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] + (seq_t)7 * cache[i - 2];
        }
        cache_size = new_cap;
    }

    return cache[n];
}

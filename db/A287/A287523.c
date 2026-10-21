/*
 * A287523  Fixed point starting with 1 of the morphism 0->01, 1->101.
 * https://oeis.org/A287523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A287523: the n‑th bit of the infinite fixed point of the
   morphism 0 → 01, 1 → 101 starting with the seed word "1".
   The sequence is built iteratively: each already known prefix is
   expanded until it covers the requested position.
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* prefix of the infinite word   */
    static idx_t cache_len = 0;      /* number of computed symbols    */

    /* initialise with the seed word "1" */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* grow the cache until it contains at least n elements */
    while (cache_len < n) {
        /* determine length of the next iteration */
        idx_t new_len = 0;
        for (idx_t i = 0; i < cache_len; ++i)
            new_len += (cache[i] == (seq_t)0) ? 2 : 3;

        /* allocate space for the expanded word */
        size_t new_len_sz = (size_t)new_len;
        seq_t *new_cache = (seq_t*)malloc(new_len_sz * sizeof(seq_t));
        if (!new_cache) abort();

        /* perform the expansion according to the morphism */
        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == (seq_t)0) {          /* 0 → 01 */
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)1;
            } else {                              /* 1 → 101 */
                new_cache[pos++] = (seq_t)1;
                new_cache[pos++] = (seq_t)0;
                new_cache[pos++] = (seq_t)1;
            }
        }

        /* drop the old buffer and remember the new one */
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* cache[n‑1] holds a(n) because cache is 0‑based */
    return cache[n - 1];
}

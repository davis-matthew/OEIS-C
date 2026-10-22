/*
 * A384150  Consecutive states of the linear congruential pseudo-random number
 *          generator (10924*s+11830) mod (2^15+1) when started at s=1.
 * https://oeis.org/A384150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (10924*a(n-1)+11830) mod 32769 (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cache[0] unused; cache[i] = a(i) */
    static idx_t cache_size = 0;      /* last computed index (≥1) */

    if (n == 0) return (seq_t)0; /* undefined for offset 1, harmless */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2); /* indices 0 and 1 */
        if (cache == NULL) abort();
        cache[0] = (seq_t)0;   /* unused */
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache_size = 1;
    }

    /* Extend cache if needed */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            sizeof(seq_t) * (n + 1));
        if (new_cache == NULL) abort();
        cache = new_cache;

        seq_t prev = cache[cache_size];
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            prev = ((seq_t)10924 * prev + (seq_t)11830) % ((seq_t)32769);
            cache[i] = prev;
        }
        cache_size = n;
    }

    return cache[n];
}

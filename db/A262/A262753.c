/*
 * A262753  Number of (n+2) X (2+2) 0..1 arrays with each row divisible by 5
 *          and each column divisible by 7, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A262753: number of (n+2)×(2+2) 0..1 arrays with each row divisible by 5
 * and each column divisible by 7, read as a binary number.
 * Empirical recurrence:
 *   a(n) = 4·a(n‑1) + 9·a(n‑3) – 36·a(n‑4) – 8·a(n‑6) + 32·a(n‑7)
 *   a(1)=4, a(2)=9, a(3)=25, a(4)=100, a(5)=361, a(6)=1369, a(7)=5476
 *
 * The function memoises results in a static cache so that any ordering of
 * calls works.  No heavy search or external libraries are required.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1…7 */
    static const seq_t init[7] = {4, 9, 25, 100, 361, 1369, 5476};

    /* cache of already computed terms (a(1) stored at index 0) */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0; /*!< number of terms stored in cache */
    static idx_t cache_cap   = 0; /*!< allocated size of cache */

    /* return immediately for the first 7 terms */
    if (n <= 7U)
        return init[n - 1U];

    /* (re)allocate cache large enough to hold the requested term */
    if (cache == NULL) {
        /* first use: allocate up to max(n,7) and copy the 7 known terms */
        cache_cap = (n > 7U) ? n : 7U;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < 7U; ++i) cache[i] = init[i];
        cache_len = 7U;
    } else if (n > cache_cap) {
        /* enlarge cache */
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {        /* i == index of a(i+1) */
        cache[i] =  4 * cache[i - 1U]
                 + 9 * cache[i - 3U]
                 - 36 * cache[i - 4U]
                 - 8  * cache[i - 6U]
                 + 32 * cache[i - 7U];
    }
    cache_len = n;

    return cache[n - 1U];
}

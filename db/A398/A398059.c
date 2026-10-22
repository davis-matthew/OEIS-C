/*
 * A398059  Upper (1,4/3) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5),
 *            a(0)=0, a(1)=3, a(2)=9, a(3)=18, a(4)=32
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- static cache for random access and memoisation --- */
    static seq_t *cache = NULL;      /* array of previously computed terms   */
    static idx_t cached_len = 0;     /* number of terms currently stored     */

    /* initialise the cache with the first five terms when called the first time */
    if (cached_len == 0) {
        const size_t init_size = 5;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)32;
        cached_len = init_size;
    }

    /* return directly if the term is already cached */
    if (n < cached_len)
        return cache[n];

    /* extend the cache up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();        /* allocation failure */
    cache = new_cache;

    /* compute missing terms using the recurrence relation */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)2;
        val -= cache[i - 2];
        val += cache[i - 3];
        val -= cache[i - 4] * (seq_t)2;
        val += cache[i - 5];
        cache[i] = val;
    }

    cached_len = n + 1;
    return cache[n];
}

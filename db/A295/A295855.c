/*
 * A295855  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = 1,
 *          a(1) = -2, a(2) = 2, a(3) = 1.
 * https://oeis.org/A295855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) – 2*a(n-3) – 2*a(n-4), a(0)=1, a(1)=–2,
 *            a(2)=2, a(3)=1
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 41, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;   /* allocated array of computed terms */
    static idx_t cur_size = 0;    /* highest index stored + 1 */

    /* Base case: n in [0,3] – return constants directly */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)-2;
            case 2: return (seq_t)2;
            case 3: return (seq_t)1;
        }
    }

    /* Allocate the cache once on first call needing it */
    if (cur_size == 0) {
        cur_size = 4;
        cache = (seq_t*)malloc(cur_size * sizeof(seq_t));
        if (!cache) abort();          /* out‑of‑memory (should not happen) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* Grow/compute the cache up to n if necessary */
    if (n >= cur_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();          /* out‑of‑memory */
        cache = new_cache;
        for (idx_t i = cur_size; i <= n; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)3 * cache[i-2]
                     - (seq_t)2 * cache[i-3]
                     - (seq_t)2 * cache[i-4];
        }
        cur_size = new_size;
    }

    return cache[n];
}

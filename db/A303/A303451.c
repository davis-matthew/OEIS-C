/*
 * A303451  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 6 horizontally, diagonally or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A303451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) - 2*a(n-3) - 36*a(n-4) - 16*a(n-5),
 *            with initial values: a(1) = 4, a(2) = 32, a(3) = 232, a(4) =
 *            1696, a(5) = 12408. (For n < 1 the function has undefined
 *            behaviour.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 1‑indexed cache; cache[0] is unused dummy */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* allocated length (including index 0) */
    static idx_t  cache_last = 0;         /* highest n computed so far */

    /* --- first call: allocate minimum space and initialise base cases ------- */
    if (cache == NULL) {
        /* allocate space for indices 0 … max(5, n) */
        size_t init_len = (size_t) (n + 2 > 6 ? n + 2 : 6);  /* at least 6 */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache_len = init_len;
        /* dummy */
        cache[0] = 0;
        /* base values up to a(5) */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)32;
        cache[3] = (seq_t)232;
        cache[4] = (seq_t)1696;
        cache[5] = (seq_t)12408;
        cache_last = 5;
    }

    /* --- extend cache if we need more space --------------------------------- */
    if ((size_t)n + 2 > cache_len) {
        size_t new_len = (size_t)n + 2;      /* enough for index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* --- compute missing terms using the recurrence ------------------------- */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* recurrence: a(i) = 8*a(i-1) - 4*a(i-2) - 2*a(i-3) - 36*a(i-4) - 16*a(i-5) */
            seq_t term = 8 * cache[i - 1];
            term -= 4 * cache[i - 2];
            term -= 2 * cache[i - 3];
            term -= 36 * cache[i - 4];
            term -= 16 * cache[i - 5];
            cache[i] = term;
        }
        cache_last = n;
    }

    return cache[n];
}

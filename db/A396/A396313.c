/*
 * A396313  Number of tilings of a 5 X n rectangle by unit squares and the
 *          Ferrers tile (3,3,2).
 * https://oeis.org/A396313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + a(n-5), a(0) = 1, n ≥ 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *cache = NULL;      /* table of computed terms       */
    static size_t   size   = 0;      /* allocated array size          */
    static size_t   cap    = 0;      /* highest index stored          */

    /* Initialise the table on the first call */
    if (!cache) {
        size = 8; /* start with a small array, will grow if needed */
        cache = (seq_t*)malloc(sizeof(seq_t) * size);
        if (!cache) return 0; /* allocation failure – not expected */
        cache[0] = (seq_t)1;   /* a(0) = 1                     */
        cap = 0;
    }

    /* Convert the index to size_t for array access */
    size_t target = (size_t)n;
    if (target <= cap) return cache[target];

    /* Grow the table if necessary */
    if (size <= target) {
        size_t new_size = size ? size * 2 : 8;
        while (new_size <= target) new_size *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;            /* allocation failure – not expected */
        cache = tmp;
        size = new_size;
    }

    /* Compute all missing terms up to target */
    for (size_t i = cap + 1; i <= target; ++i) {
        seq_t val = cache[i - 1];
        if (i >= 3) val += (seq_t)3 * cache[i - 3];
        if (i >= 5) val += cache[i - 5];
        cache[i] = val;
    }
    cap = target;
    return cache[target];
}

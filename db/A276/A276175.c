/*
 * A276175  a(n) = (a(n-1)+1)*(a(n-2)+1)*(a(n-3)+1)/a(n-4) with a(0) = a(1) =
 *          a(2) = a(3) = 1.
 * https://oeis.org/A276175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276175: a(n) = (a(n-1)+1)*(a(n-2)+1)*(a(n-3)+1)/a(n-4),
   with a(0) = a(1) = a(2) = a(3) = 1 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size */
    static idx_t last = 0;   /* highest index computed (inclusive) */

    /* initialise cache with first four terms if not yet done */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
        last = 3;
    }

    if (n <= last)
        return cache[n];

    /* grow the cache to accommodate n */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= n)
            new_cap = (new_cap == 0) ? 4 : new_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failed – abort */
            exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t x1 = cache[i-1] + (seq_t)1;
        seq_t x2 = cache[i-2] + (seq_t)1;
        seq_t x3 = cache[i-3] + (seq_t)1;
        seq_t denom = cache[i-4];
        cache[i] = (x1 * x2 * x3) / denom;
    }
    last = n;
    return cache[n];
}

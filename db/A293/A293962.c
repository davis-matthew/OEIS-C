/*
 * A293962  Number of linear chord diagrams having n chords and maximal chord
 *          length n, a(0)=1.
 * https://oeis.org/A293962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 2, * a(n) = (2*n - 1) * a(n-1) for n
 *            > 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of previously computed terms */
    static size_t cap   = 0;      /* current capacity of the array */
    static idx_t  max_n = 0;      /* largest index currently stored */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap = 3;                    /* we store indices 0, 1, 2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)         /* allocation failure – give up */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        max_n   = 2;
    }

    /* compute missing terms if needed */
    if (n > max_n) {
        /* grow the array if necessary */
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)           /* allocation failure – give up */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;

        /* fill in terms from max_n+1 up to n */
        idx_t start = max_n + 1;
        if (start < 3) start = 3;   /* base terms already present */
        for (idx_t i = start; i <= n; ++i) {
            seq_t mult = (seq_t)(2 * i - 1);
            cache[(size_t)i] = mult * cache[(size_t)(i-1)];
        }
        max_n = n;
    }

    return cache[(size_t)n];
}

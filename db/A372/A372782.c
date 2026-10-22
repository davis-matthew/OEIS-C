/*
 * A372782  Least number m for which there exists some positive k < m where
 *          the sum of the integers from k + 1 to m inclusive is an n-th power
 *          > 1.
 * https://oeis.org/A372782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + 1, with a(1)=2, a(2)=4
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoisation to handle out‑of‑order calls efficiently. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return a sentinel for the unused index 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of computed values. */
    static seq_t *cache = NULL;
    static size_t size  = 0;   /* largest index computed so far */
    static size_t capa  = 0;   /* current capacity of cache */

    if (n > size) {
        /* Allocate / expand the cache to hold indices up to n. */
        size_t need = n + 1;               /* +1 to use 1‑based indexing */
        while (capa < need) {
            size_t newcapa = capa ? capa * 2 : 4;
            if (newcapa < need)
                newcapa = need;
            seq_t *tmp = (seq_t *)realloc(cache, newcapa * sizeof(seq_t));
            if (!tmp) {           /* allocation failure: terminate */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            capa  = newcapa;
        }

        /* Compute and store missing terms using the recurrence. */
        for (size_t i = size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)2;
            } else if (i == 2) {
                cache[i] = (seq_t)4;
            } else {
                seq_t val = cache[i - 2];
                val = val * (seq_t)3 + (seq_t)1;
                cache[i] = val;
            }
        }
        size = n;
    }

    return cache[n];
}

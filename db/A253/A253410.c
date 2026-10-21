/*
 * A253410  Indices of centered pentagonal numbers (A005891) which are also
 *          centered octagonal numbers (A016754).
 * https://oeis.org/A253410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1442*a(n-2) - 1442*a(n-3) - a(n-4) + a(n-5)
 *            Initial values: a(1)=1, a(2)=96, a(3)=817, a(4)=137712,
 *            a(5)=1177393
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cache for already computed terms */
    static seq_t *cache = NULL;   /* zero-initialized */
    static size_t  cap   = 0;     /* current cache capacity */
    /* Protect against n being zero (offset starts at 1) */
    if (n == 0) return 0;

    /* Ensure the cache can hold up to n terms */
    if ((size_t)n > cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failed */
        /* Initialise new elements to zero (default constructor for big types) */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first five terms once */
    if (cache[0] == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)96;
        cache[2] = (seq_t)817;
        cache[3] = (seq_t)137712;
        cache[4] = (seq_t)1177393;
    }

    /* Compute all missing terms up to the requested index (zero‑based) */
    for (size_t i = 5; i < (size_t)n; ++i) {
        cache[i] =
            cache[i-1]
          + (seq_t)1442 * cache[i-2]
          - (seq_t)1442 * cache[i-3]
          - cache[i-4]
          + cache[i-5];
    }

    return cache[(size_t)n - 1];
}

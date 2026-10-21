/*
 * A283858  Number of 2 X n 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 7*a(n-3) + 6*a(n-4) (n≥5); initial
 *            values: a(1)=4, a(2)=16, a(3)=61, a(4)=233
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    static seq_t *cache = NULL;          /* values 1..cache_size */
    static size_t cache_size = 0;        /* highest n computed */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* allocate and initialize cache on first call */
    if (!cache) {
        cache_cap = 16;                  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failure */
        cache[0] = (seq_t)4;             /* a(1) */
        cache[1] = (seq_t)16;            /* a(2) */
        cache[2] = (seq_t)61;            /* a(3) */
        cache[3] = (seq_t)233;           /* a(4) */
        cache_size = 4;
    }

    /* enlarge cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* value already computed? */
    if ((size_t)n <= cache_size)
        return cache[(size_t)(n - 1)];

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) + a(i-2) + 7*a(i-3) + 6*a(i-4) */
        seq_t val =
          (seq_t)3 * cache[(size_t)(i - 2)] +        /* 3*a(i-1) */
          cache[(size_t)(i - 3)] +                   /* + a(i-2)  */
          (seq_t)7 * cache[(size_t)(i - 4)] +        /* + 7*a(i-3)*/
          (seq_t)6 * cache[(size_t)(i - 5)];         /* + 6*a(i-4)*/
        cache[(size_t)(i - 1)] = val;
        ++cache_size;
    }

    return cache[(size_t)(n - 1)];
}

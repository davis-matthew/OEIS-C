/*
 * A292492  p-INVERT of the odd positive integers, where p(S) = 1 - S + S^2 -
 *          S^3.
 * https://oeis.org/A292492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 19*a(n-2) + 23*a(n-3) - 8*a(n-4) + 6*a(n-5)
 *            for n >= 7 initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=8,
 *            a(4)=22, a(5)=100, a(6)=444
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements currently stored */

    /* initialize first 7 terms once */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failed */

        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 5;
        cache[3] = 8;
        cache[4] = 22;
        cache[5] = 100;
        cache[6] = 444;
    }

    /* return value if already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        size_t new_size = i + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;

        cache[i] =
            7 * cache[i - 1]
          - 19 * cache[i - 2]
          + 23 * cache[i - 3]
          - 8  * cache[i - 4]
          + 6  * cache[i - 5];

        cache_size = new_size;
    }

    return cache[n];
}

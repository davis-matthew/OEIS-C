/*
 * A255451  A255450(2^n-1).
 * https://oeis.org/A255451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=21, a(3)=85, and for n≥4: a(n) = 4*a(n-1) +
 *            a(n-2) – 4*a(n-3) – 8*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 0..len-1 are known */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  len   = 0;

    /* grow the cache if necessary */
    size_t new_cap = cap ? cap : 4;           /* at least 4 elements */
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure (unlikely) */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialize the first four terms on first use */
    if (len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)85;
        len = 4;
    }

    /* return the desired term if already computed */
    if (len > n) return cache[n];

    /* compute forward from len up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t v = (seq_t)4 * cache[i-1] +
                  cache[i-2] -
                  (seq_t)4 * cache[i-3] -
                  (seq_t)8 * cache[i-4];
        cache[i] = v;
    }
    len = n + 1;
    return cache[n];
}

/*
 * A275638  Expansion of (1-q)^k/Product_{j=1..k} (1-q^j) for k=4.
 * https://oeis.org/A275638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(3*a(n-1)+5*a(n-2)+6*a(n-3)+5*a(n-4)+3*a(n-5)+a(n-6))
 *            with a(0)= 1, a(1)= -3, a(2)= 4, a(3)= -3, a(4)= 2, a(5)= -3
 * Range:     i8 n <= 252, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache of computed terms               */
static size_t cache_len = 0;     /* number of valid terms in cache        */
static size_t cache_cap = 0;     /* allocated capacity of cache           */

seq_t a(idx_t n)
{
    /* initialise cache with first six terms */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – give up */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)-3;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)-3;
        cache_len = 6;
    }

    /* expand cache if required */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* out of memory – give up */
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t s = (seq_t)3 * cache[i-1] +
                      (seq_t)5 * cache[i-2] +
                      (seq_t)6 * cache[i-3] +
                      (seq_t)5 * cache[i-4] +
                      (seq_t)3 * cache[i-5] +
                      cache[i-6];
            cache[i] = -s;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}

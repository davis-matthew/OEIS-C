/*
 * A274952  Expansion of (4+x-x^2+x^3-x^4+x^5) / (1-3*x-x^2+x^3-x^4+x^5-x^6).
 * https://oeis.org/A274952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n)=3*a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)+a(n-6),
   a(0)=4, a(1)=13, a(2)=42, a(3)=136, a(4)=440, a(5)=1424 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_len = 0;    /* number of valid terms stored */
    static size_t cache_cap = 0;    /* current allocation size */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 6;              /* capacity for indices 0..5 */
        cache_len = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)42;
        cache[3] = (seq_t)136;
        cache[4] = (seq_t)440;
        cache[5] = (seq_t)1424;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    if ((size_t)n >= cache_len) {
        size_t start = cache_len;          /* first index to compute */
        cache_len = (size_t)n + 1;         /* new number of valid terms */
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t val = cache[i-1] * (seq_t)3;
            val += cache[i-2];
            val -= cache[i-3];
            val += cache[i-4];
            val -= cache[i-5];
            val += cache[i-6];
            cache[i] = val;
        }
    }

    return cache[(size_t)n];
}

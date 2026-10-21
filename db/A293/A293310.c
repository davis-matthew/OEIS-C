/*
 * A293310  Number of magic labelings of the graph LOOP X C_9 (see comments)
 *          having magic sum n, n >= 0.
 * https://oeis.org/A293310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) -35*a(n-2) +75*a(n-3) -90*a(n-4) +42*a(n-5)
 *            +42*a(n-6) -90*a(n-7) +75*a(n-8) -35*a(n-9) +9*a(n-10) - a(n-11)
 *            (n ≥ 11) with a(0) = 1, a(1) = 76, a(2) = 1460, a(3) = 13604,
 *            a(4) = 81555, a(5) = 363606, a(6) = 1310974, a(7) = 4029310,
 *            a(8) = 10936124, a(9) = 26868719, a(10) = 60843972
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* initial terms a(0) … a(10) */
    static const seq_t init[11] = {
        (seq_t)1,
        (seq_t)76,
        (seq_t)1460,
        (seq_t)13604,
        (seq_t)81555,
        (seq_t)363606,
        (seq_t)1310974,
        (seq_t)4029310,
        (seq_t)10936124,
        (seq_t)26868719,
        (seq_t)60843972
    };

    /* small indices handled directly */
    if (n < 11U) {
        return init[(size_t)n];
    }

    /* memoisation buffer */
    static seq_t *cache = NULL;
    static size_t size = 0;    /* number of elements currently stored */

    /* initialise cache if this is the first request for n ≥ 11 */
    if (cache == NULL) {
        size = 11;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache) abort();                 /* out of memory */
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init[i];
        }
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > size) {
        size_t newsize = size * 2;
        if (newsize < (size_t)n + 1)
            newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
        if (!tmp) abort();                  /* out of memory */
        cache = tmp;
        size = newsize;
    }

    /* compute missing values up to n (recurrence valid for n ≥ 11) */
    size_t start = 11;                     /* indices 0 … 10 are already present */
    if (size < start) start = size;        /* safety: should not happen */
    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t r = mul_small(cache[i - 1], 9);          /* 9*a(n-1) */
        r -= mul_small(cache[i - 2], 35);             /* -35*a(n-2) */
        r += mul_small(cache[i - 3], 75);             /* +75*a(n-3) */
        r -= mul_small(cache[i - 4], 90);             /* -90*a(n-4) */
        r += mul_small(cache[i - 5], 42);             /* +42*a(n-5) */
        r += mul_small(cache[i - 6], 42);             /* +42*a(n-6) */
        r -= mul_small(cache[i - 7], 90);             /* -90*a(n-7) */
        r += mul_small(cache[i - 8], 75);             /* +75*a(n-8) */
        r -= mul_small(cache[i - 9], 35);             /* -35*a(n-9) */
        r += mul_small(cache[i -10], 9);              /* +9*a(n-10) */
        r -= cache[i -11];                            /* -a(n-11) */
        cache[i] = r;
    }

    return cache[(size_t)n];
}

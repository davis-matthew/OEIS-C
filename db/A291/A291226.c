/*
 * A291226  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)^6.
 * https://oeis.org/A291226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 10*a(n-3) + 30*a(n-4) + 6*a(n-5) -
 *            41*a(n-6) - 6*a(n-7) + 30*a(n-8) + 10*a(n-9) - 9*a(n-10) -
 *            6*a(n-11) - a(n-12) Initial values: a(0)=6, a(1)=21, a(2)=62,
 *            a(3)=168, a(4)=426, a(5)=1029, a(6)=2394, a(7)=5403, a(8)=11892,
 *            a(9)=25626, a(10)=54228, a(11)=112958, a(12)=23...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 27, u64 n <= 65, u128 n <= 150,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"   /* mul_small(seq_t x, unsigned int m) */

static const seq_t _init[13] = {
    (seq_t)6,   (seq_t)21,  (seq_t)62,   (seq_t)168,
    (seq_t)426, (seq_t)1029,(seq_t)2394, (seq_t)5403,
    (seq_t)11892,(seq_t)25626,(seq_t)54228,(seq_t)112958,
    (seq_t)232056
};

seq_t a(idx_t n)
{
    /* Lazy memoisation: cache computed values in a static array. */
    static seq_t *cache = NULL;
    static size_t  cache_size  = 0;
    static idx_t   computed_max = 0;   /* largest n for which cache[n] is valid */

    /* Initialise cache on first call or grow it if needed. */
    size_t required = (size_t)n + 1;
    if (required <= 12) required = 13;        /* need space up to index 12 */
    if (cache == NULL) {
        cache_size = required;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i <= 12; ++i) cache[i] = _init[i];
        for (size_t i = 13; i < cache_size; ++i) cache[i] = 0;
        computed_max = 12;
    } else if (required > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < required) new_size = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed, return 0 as fallback */
        cache = tmp;
        /* initialise any new entries beyond old size */
        for (size_t i = cache_size; i <= required - 1; ++i) {
            if (i <= 12) cache[i] = _init[i];
            else         cache[i] = 0;
        }
        cache_size = new_size;
    }

    if (n <= computed_max) return cache[n];

    /* Compute subsequent values using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += mul_small(cache[i-1], 6);
        pos += mul_small(cache[i-4], 30);
        pos += mul_small(cache[i-5], 6);
        pos += mul_small(cache[i-8], 30);
        pos += mul_small(cache[i-9], 10);

        /* Negative contributions */
        neg += mul_small(cache[i-2], 9);
        neg += mul_small(cache[i-3], 10);
        neg += mul_small(cache[i-6], 41);
        neg += mul_small(cache[i-7], 6);
        neg += mul_small(cache[i-10], 9);
        neg += mul_small(cache[i-11], 6);
        neg += cache[i-12];   /* -a(n-12) */

        cache[i] = pos - neg;   /* guaranteed non‑negative for the sequence */
    }

    computed_max = n;
    return cache[n];
}

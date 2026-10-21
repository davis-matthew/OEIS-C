/*
 * A276018  n^2 * a(n) = 3*(3*n-2)^2 * a(n-1), with a(0) = 1.
 * https://oeis.org/A276018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276018: a(0) = 1, and for n ≥ 1
   n^2 * a(n) = 3*(3*n-2)^2 * a(n-1).  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    /* Grow the cache to hold indices up to n */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_len = 0;
    } else if (cache_len <= (size_t)n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            idx_t t = 3 * i - 2;
            seq_t num = (seq_t)3 * (seq_t)t * (seq_t)t;   /* 3*(3*i-2)^2 */
            seq_t den = (seq_t)i * (seq_t)i;              /* i^2           */

            /* Reduce the fraction to avoid overflow */
            seq_t g = gcd(cache[i-1], den);
            seq_t a1 = cache[i-1] / g;
            den /= g;

            g = gcd(num, den);
            seq_t num1 = num / g;
            den /= g;  /* den should now divide a1 exactly */

            seq_t a2 = a1 / den;
            cache[i] = a2 * num1;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

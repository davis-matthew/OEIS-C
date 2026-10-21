/*
 * A275861  a(n) = floor(c*s*a(n-1)) + floor(d*r*a(n-2)), where r = 2 +
 *          sqrt(5), s = r/(r-1), c = 4, d = 1, a(0) = 1, a(1) = 1.
 * https://oeis.org/A275861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275861 via its order‑7 linear recurrence
   a(n) = 7*a(n-1) - 7*a(n-2) + 5*a(n-3) - 3*a(n-4) - 3*a(n-5) + 3*a(n-6) - a(n-7)
   with a(0)=1, a(1)=1, a(2)=9, a(3)=51, a(4)=305, a(5)=1813, a(6)=10784.
   This implements the sequence for any build where seq_t is an unsigned
   integer type (8/16/32/64/128 bits or an arbitrary‑precision class). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* cache holds all already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of values in cache */

    /* extend cache to fit index n */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_len? cache_len : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – not expected */
        cache = tmp;
    }

    /* initial seven terms */
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)9,
        (seq_t)51,
        (seq_t)305,
        (seq_t)1813,
        (seq_t)10784
    };

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 7) {
            cache[i] = init[i];
        } else {
            /* positive part: 7*a(i-1) + 5*a(i-3) + 3*a(i-6) */
            seq_t pos = 7 * cache[i-1];
            pos += 5 * cache[i-3];
            pos += 3 * cache[i-6];

            /* negative part: 7*a(i-2) + 3*a(i-4) + 3*a(i-5) + 1*a(i-7) */
            seq_t neg = 7 * cache[i-2];
            neg += 3 * cache[i-4];
            neg += 3 * cache[i-5];
            neg += cache[i-7];

            cache[i] = pos - neg;  /* positive_sum >= negative_sum for all n */
        }
    }

    cache_len = n + 1;          /* cache now contains indices 0..n inclusive */
    return cache[n];
}

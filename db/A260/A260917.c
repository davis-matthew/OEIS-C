/*
 * A260917  Expansion of 1/(1 - x - x^2 - x^3 + x^6 + x^7).
 * https://oeis.org/A260917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-6) - a(n-7), with initial
 *            values a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=13, a(6)=23
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Grow cache if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);          /* safe for n up to 10000 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) /* allocation failure → abort */
            abort();
        cache = new_cache;
    }

    /* Compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)1;
        else if (i == 2)     cache[i] = (seq_t)2;
        else if (i == 3)     cache[i] = (seq_t)4;
        else if (i == 4)     cache[i] = (seq_t)7;
        else if (i == 5)     cache[i] = (seq_t)13;
        else if (i == 6)     cache[i] = (seq_t)23;
        else {
            /* a(n) = a(n-1)+a(n-2)+a(n-3) – a(n-6) – a(n-7) */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
            cache[i] = sum - cache[i-6] - cache[i-7];
        }
    }
    cache_len = n + 1;
    return cache[n];
}

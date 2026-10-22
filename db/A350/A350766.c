/*
 * A350766  Reversed sum of the two previous terms, with a(1) = 1 and a(2) =
 *          11.
 * https://oeis.org/A350766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = rev_decimal(a(n-1)+a(n-2)); a(1)=1; a(2)=11
 * Range:     u8 n <= 14, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper to reverse decimal digits of a seq_t value */
static seq_t rev_digits(seq_t v) {
    seq_t r = 0;
    while (v > 0) {
        r = r * 10 + (v % 10);
        v = v / 10;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1; value for 0 is unused */

    /* Static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity */
    static size_t cache_len = 0;          /* number of terms stored */

    size_t needed = (size_t)n;

    /* Ensure the cache is large enough */
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap * 2 : 2;
        if (newcap < needed) newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure (unlikely) */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Initialize first two terms if not already done */
    if (cache_len < 2) {
        if (cache_cap < 2) { /* should never happen due to the allocation above */ }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache_len = 2;
    }

    /* Return already computed term if available */
    if (needed <= cache_len) return cache[n - 1];

    /* Compute missing terms up to the requested index */
    for (size_t i = cache_len; i < needed; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = rev_digits(sum);
    }
    cache_len = needed;

    return cache[n - 1];
}

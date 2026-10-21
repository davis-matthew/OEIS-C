/*
 * A295333  Numerators of continued fraction convergents to sqrt(10)/2 =
 *          sqrt(5/2) = A020797 + 1.
 * https://oeis.org/A295333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = 6*a(n-3) + a(n-6) for n>=6, with a(0)=1, a(1)=2, a(2)=3,
 *            a(3)=8, a(4)=11, a(5)=19
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization array */
static size_t cache_len = 0;         /* number of computed entries */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure that cache can hold indices up to 'need'.  Grow by doubling. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort(); /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    /* At least need indices 0..idx */
    ensure_capacity(idx + 1);

    /* First call: initialise first six values */
    if (cache_len == 0) {
        /* Guarantee capacity >= 6 */
        if (cache_cap < 6)
            ensure_capacity(6);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)19;
        cache_len = 6;
    }

    /* If the requested term is not yet computed, compute it */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i < 6) continue; /* already initialised */
            cache[i] = cache[i-3] * (seq_t)6 + cache[i-6];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}

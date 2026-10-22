/*
 * A323210  a(n) = 9*J(n)^2 where J(n) are the Jacobsthal numbers A001045 with
 *          J(0) = 1.
 * https://oeis.org/A323210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * J(n)^2, with J(n) the Jacobsthal numbers (J(0)=0,
 *            J(1)=1). For n>0 this equals 9 * ((2^n - (-1)^n)/3)^2.
 *            Recurrence: a(0)=1, a(1)=9, a(2)=9, a(3)=81, a(n) =
 *            3*a(n-1)+6*a(n-2)-8*a(n-3) for n>=4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialize cache on first use with the first four terms */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* out-of-memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)81;
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to accommodate index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out-of-memory guard */
        cache = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms using the linear recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        cache[i] = 3 * a1 + 6 * a2 - 8 * a3;
    }

    cache_len = needed;
    return cache[(size_t)n];
}

/*
 * A384634  Expansion of (1+2*x+x^2) / (1-6*x^2-4*x^3+2*x^4).
 * https://oeis.org/A384634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(3)=16 * Recurrence for n≥4: a(n) =
 *            6*a(n‑2) + 4*a(n‑3) – 2*a(n‑4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static state for memoisation */
static seq_t *cache = NULL;   /* array of computed terms   */
static size_t cache_cap = 0; /* allocated capacity (count) */
static size_t cache_len = 0; /* number of terms currently stored (first = 0, last = len-1) */

/* Ensure the cache can store indices 0..n */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed < 4) needed = 4;               /* keep space for the first four terms */
    if (!cache) {
        /* allocate the array */
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_cap = needed;
        return;
    }
    if (needed > cache_cap) {
        seq_t *newarr = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache = newarr;
        cache_cap = needed;
    }
}

/* Return the nth term of A384634 */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* initialise the first four terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)16;
        cache_len = 4;
    }

    if ((size_t)n < cache_len) {
        return cache[(size_t)n];
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t t2 = cache[(size_t)(i - 2)];
        t2 *= (seq_t)6;
        seq_t t3 = cache[(size_t)(i - 3)];
        t3 *= (seq_t)4;
        seq_t t4 = cache[(size_t)(i - 4)];
        t4 *= (seq_t)2;
        cache[(size_t)i] = t2 + t3 - t4;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

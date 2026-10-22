/*
 * A390349  Upper (1/2)-midsequence of Fibonacci numbers (A000045) and
 *          tribonacci numbers (A000213); see Comments.
 * https://oeis.org/A390349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-4) - 2*a(n-5) + a(n-6) +
 *            2*a(n-7) + a(n-8), with initial values (a(0)...a(7)) = (1, 1, 1,
 *            3, 4, 7, 13, 22)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cache for computed terms */
static size_t cache_len = 0;       /* number of cached terms                    */
static size_t cache_cap = 0;       /* current capacity of cache array            */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);               /* out‑of‑memory: terminate */
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 1, 1, 3, 4, 7, 13, 22};

    size_t idx = (size_t)n;          /* cast, safe on typical 64‑bit builds */

    /* Grow cache if necessary. */
    ensure_cache_cap(idx + 1);

    /* If already computed, return. */
    if (idx < cache_len) return cache[idx];

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t val;
        if (i < 8) {
            val = init[i];
        } else {
            /* Positive contributions */
            seq_t pos = cache[i - 1] << 1;          /* 2*a(i-1) */
            pos += cache[i - 2];
            pos += cache[i - 6];
            pos += cache[i - 7] << 1;              /* 2*a(i-7) */
            pos += cache[i - 8];

            /* Negative contributions */
            seq_t neg = (seq_t)4 * cache[i - 4];
            neg += (seq_t)2 * cache[i - 5];

            val = pos - neg;                       /* pos >= neg by sequence definition */
        }
        cache[i] = val;
    }
    cache_len = idx + 1;
    return cache[idx];
}

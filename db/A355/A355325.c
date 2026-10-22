/*
 * A355325  Upper midsequence of the Fibonacci numbers (1,2,3,5,8,...) and
 *          Lucas numbers (1,3,4,7,11,...); see Comments.
 * https://oeis.org/A355325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355325: Upper midsequence of Fibonacci and Lucas numbers
 *
 * Recurrence:  a(n) = a(n‑1) + a(n‑2) + a(n‑3) – a(n‑4) – a(n‑5)
 * Initial values:  a(0)=1, a(1)=3, a(2)=4, a(3)=6, a(4)=10
 *
 * The function memoises computed values for fast repeated access.
 * All arithmetic is performed in seq_t, which for 64‑bit builds
 * is unsigned long long; intermediate sums are safe for all n
 * whose results fit in seq_t.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoization */
static seq_t *cache      = NULL;   /* pointer to cached values */
static size_t cache_cap = 0;       /* allocated capacity (in elements) */
static size_t cache_len = 0;       /* number of initialized elements */

/* Ensure the cache holds at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* allocation failure */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Compute values up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    size_t target = (size_t)n + 1;
    ensure_capacity(target);

    /* Initialise base values on first call. */
    if (cache_len == 0) {
        /* Make sure there is space for the initial 5 terms. */
        if (cache_cap < 5)
            ensure_capacity(5);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)10;
        cache_len = 5;
    }

    /* If we already have the requested index, nothing to do. */
    if (cache_len > (size_t)n)
        return;

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3]
                  - cache[i-4] - cache[i-5];
    }

    cache_len = target;
}

/* Public function required by the evaluation harness */
seq_t a(idx_t n)
{
    if (cache_len <= (size_t)n)
        compute_up_to(n);
    return cache[(size_t)n];
}

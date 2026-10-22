/*
 * A372865  a(n) = (Sum_{k = 1..n-1} k*a(k)) (mod n) with a(1) = 1.
 * https://oeis.org/A372865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∑_{k = 1..n-1} k*a(k)) (mod n), a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 73, u32 n <= 2960, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  O(n) incremental computation with memoization. */

#include <stdlib.h>

/* External types provided by the build: idx_t and seq_t. */
static seq_t *a_cache = NULL;    /* array of computed a(n), 1‑based */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  computed_to = 0;   /* highest n computed so far */
static seq_t  cum_sum   = 0;     /* ∑_{k=1}^{m} k*a(k) for m=computed_to */

/* Ensure the cache can hold indices up to n (1‑based). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)n < cache_cap) return;          /* already enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if ((size_t)n + 1 > new_cap) new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();                    /* allocation failure */
    /* zero‑initialize new slots if any */
    if (new_cap > cache_cap)
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    a_cache   = new_cache;
    cache_cap = new_cap;
}

/* Main function: compute a(n) for any n >= 1, in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined only for n≥1 */
    if (n == 1) {
        /* ensure base case is stored */
        if (computed_to < 1) {
            ensure_cache_cap(1);
            a_cache[1] = 1;
            cum_sum   = 1;            /* sum_{k=1}^{1} k*a(k) = 1*1 */
            computed_to = 1;
        }
        return a_cache[1];
    }

    if (n > computed_to) {
        ensure_cache_cap(n);
        /* compute terms from computed_to+1 up to n */
        for (idx_t i = computed_to + 1; i <= n; ++i) {
            seq_t ai = cum_sum % (seq_t)i;     /* a(i) = sum_{k< i} k*a(k) (mod i) */
            a_cache[i] = ai;
            cum_sum += (seq_t)i * ai;          /* update cumulative sum */
        }
        computed_to = n;
    }

    return a_cache[n];
}

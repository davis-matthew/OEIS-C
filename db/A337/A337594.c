/*
 * A337594  a(0) = 1; a(n) = (1/n) * Sum_{k=1..n} binomial(n,k)^2 * k *
 *          4^(k-1) * a(n-k).
 * https://oeis.org/A337594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/n) * Σ_{k=1..n} binomial(n,k)^2 * k *
 *            4^(k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"   /* provides seq_alloc   */
#include "../utils/seq_realloc.h" /* provides seq_realloc */

static seq_t *cache = NULL;      /* memoisation table: cache[i] = a(i)   */
static idx_t cache_size = 0;     /* current table size                  */
static idx_t computed_max = 0;   /* largest index that has been computed */

/* Ensure cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        if (cache == NULL) {
            cache = seq_alloc(new_size);
        } else {
            cache = seq_realloc(cache, cache_size, new_size);
        }
        /* zero‑initialise the newly added entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_size = new_size;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* make sure the cache can address index n */
    ensure_cache(n);

    /* a(0) is always 1 */
    if (n == 0) {
        cache[0] = (seq_t)1;
        computed_max = 0;
        return cache[0];
    }

    /* compute any missing values up to n (if already done, skip) */
    idx_t start = computed_max + 1;
    if (start < 1) start = 1;

    for (idx_t i = start; i <= n; ++i) {
        /* ------------------------------------------------------------------ */
        /*  Compute a(i) using the recurrence                                      */
        /* ------------------------------------------------------------------ */
        /* use 128‑bit integer to accumulate the sum to avoid overflow          */
        unsigned __int128 sum = 0;
        /* bin holds binomial(i, k-1); initialise for k=0 */
        seq_t bin = (seq_t)1;
        /* 4^(k-1) for k=1 is 1 */
        unsigned __int128 pow4 = 1;

        for (idx_t k = 1; k <= i; ++k) {
            /* update bin to binomial(i, k) */
            bin = bin * ((seq_t)(i - k + 1));
            bin = bin / ((seq_t)k);          /* exact division */

            /* term = bin^2 * k * 4^(k-1) * a(i-k) */
            unsigned __int128 term = (unsigned __int128)bin * (unsigned __int128)bin; /* bin^2 */
            term *= (unsigned __int128)k;
            term *= pow4;
            term *= (unsigned __int128)cache[i - k];

            sum += term;

            /* advance 4^(k-1) -> 4^k for next k */
            pow4 *= 4u;
        }

        /* division by i (exact) */
        cache[i] = (seq_t)(sum / (unsigned __int128)i);
        /* update cache if this is a new maximum */
        if (i > computed_max) computed_max = i;
    }

    /* f(n) is now in cache[n] */
    return cache[n];
}

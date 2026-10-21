/*
 * A268258  Number of length-(n+1) 0..5 arrays with new repeated values
 *          introduced in sequential order starting with zero.
 * https://oeis.org/A268258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 390*a(n-2) + 2490*a(n-3) - 7960*a(n-4) * +
 *            8610*a(n-5) + 11775*a(n-6) - 18175*a(n-7) - 22024*a(n-8) * -
 *            7236*a(n-9) - 720*a(n-10) * Initial values: a(1) = 31, a(2) =
 *            161, a(3) = 837, * a(4) = 4357, a(5) = 22710, a(6) = 118530, *
 *            a(7) = 619490, a(8) = 3242265, a(9) = 16993552, *...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached sequence values */
static size_t cache_len  = 0;      /* number of entries stored */
static size_t cache_cap  = 0;      /* current allocation capacity */

static const seq_t init_vals[10] = {
    (seq_t)31,
    (seq_t)161,
    (seq_t)837,
    (seq_t)4357,
    (seq_t)22710,
    (seq_t)118530,
    (seq_t)619490,
    (seq_t)3242265,
    (seq_t)16993552,
    (seq_t)89197862
};

/* Ensure that cache contains values up to at least index n (1‑based). */
static void ensure_cache_up_to(idx_t n)
{
    if (n < 1) return;   /* undefined; ignore */

    if (cache_len == 0) {
        /* first time: allocate space for the initial 10 values */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failure; terminate program */
            abort();
        }
        for (size_t i = 0; i < 10; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 10;
    }

    /* Grow cache if needed */
    if ((idx_t)n <= cache_len) return;

    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* compute a(k) using the recurrence, splitting positive
         * and negative contributions to avoid signed arithmetic. */
        size_t idx = (size_t)(k - 1); /* 0‑based index */

        seq_t pos = (seq_t)31 * cache[idx - 1]   /* a(k-1) */
                  + (seq_t)2490 * cache[idx - 3] /* a(k-3) */
                  + (seq_t)8610 * cache[idx - 5] /* a(k-5) */
                  + (seq_t)11775 * cache[idx - 6];/* a(k-6) */

        seq_t neg = (seq_t)390 * cache[idx - 2]   /* a(k-2) */
                  + (seq_t)7960 * cache[idx - 4] /* a(k-4) */
                  + (seq_t)18175 * cache[idx - 7]/* a(k-7) */
                  + (seq_t)22024 * cache[idx - 8]/* a(k-8) */
                  + (seq_t)7236 * cache[idx - 9] /* a(k-9) */
                  + (seq_t)720 * cache[idx -10]; /* a(k-10) */

        seq_t val = pos - neg;   /* pos ≥ neg for all n ≥ 11 */

        /* Ensure sufficient capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!newp) abort();
            cache = newp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined; safe return */
    ensure_cache_up_to(n);
    return cache[n - 1];
}

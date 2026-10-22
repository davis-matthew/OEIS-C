/*
 * A305561  Expansion of 2*x*(1 - 2*x)/(1 + 2*x - 8*x^2 - sqrt(1 - 4*x^2)).
 * https://oeis.org/A305561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A305561 – Expansion of 2*x*(1 - 2*x)/(1 + 2*x - 8*x^2 - sqrt(1 - 4*x^2)).
 *
 * Formula (recurrence, n ≥ 5):
 *   n*(n+1)*a(n) + (n-1)*(n-5)*a(n-1) - 12*(n-1)*(n+1)*a(n-2)
 *   - 12*(n-2)*(n-5)*a(n-3) + 32*(n+1)*(n-3)*a(n-4) + 32*(n-4)*(n-5)*a(n-5) = 0
 *
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=3, a(3)=8, a(4)=23
 *
 * The recurrence is implemented with only unsigned arithmetic on seq_t.
 * Values are memoised so that arbitrary‑order requests work correctly.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache of computed a(n) */
static size_t cache_size = 0;   /* number of entries in a_cache */

/* Expand the cache to at least n+1 entries and compute missing values */
static void
ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    /* Grow the cache */
    seq_t *new_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – terminate to avoid undefined behaviour */
        abort();
    }
    a_cache = new_cache;

    /* Compute new values */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)
            a_cache[0] = (seq_t)1;
        else if (i == 1)
            a_cache[1] = (seq_t)1;
        else if (i == 2)
            a_cache[2] = (seq_t)3;
        else if (i == 3)
            a_cache[3] = (seq_t)8;
        else if (i == 4)
            a_cache[4] = (seq_t)23;
        else {
            /* Recurrence for n = i (≥5) */
            seq_t A = a_cache[i - 1];
            seq_t B = a_cache[i - 2];
            seq_t C = a_cache[i - 3];
            seq_t D = a_cache[i - 4];
            seq_t E = a_cache[i - 5];

            seq_t c0 = (seq_t)i * (seq_t)(i + 1);
            seq_t c1 = (seq_t)(i - 1) * (seq_t)(i - 5);
            seq_t c2_abs = (seq_t)12 * (seq_t)(i - 1) * (seq_t)(i + 1);
            seq_t c3_abs = (seq_t)12 * (seq_t)(i - 2) * (seq_t)(i - 5);
            seq_t c4 = (seq_t)32 * (seq_t)(i + 1) * (seq_t)(i - 3);
            seq_t c5 = (seq_t)32 * (seq_t)(i - 4) * (seq_t)(i - 5);

            seq_t left  = c2_abs * B + c3_abs * C;
            seq_t right = c1 * A + c4 * D + c5 * E;
            seq_t numerator = left - right;  /* guaranteed non‑negative */

            a_cache[i] = numerator / c0;      /* exact division by recurrence */
        }
    }

    cache_size = n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}

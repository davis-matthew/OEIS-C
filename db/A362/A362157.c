/*
 * A362157  Expansion of -1/(1 - x * (1-9*x)^(1/3)).
 * https://oeis.org/A362157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = -∑_{k=0..n} (-9)^k * binomial((n-k)/3, k)
 * Recurrence (derived from the generating function):
 *
 *    a(0) = -1
 *    c(0) =  1
 *    c(k) =  c(k‑1) * (3*(3k‑4)) / k          for k ≥ 1
 *    a(n) = ∑_{k=0}^{n-1} a(n-1‑k) * c(k)   for n ≥ 1
 * 
 * The implementation memoises the two sequences {a(n)} and {c(n)}.
 * All arithmetic is performed in a larger native type (__int128) to
 * avoid overflow of the final  result when it still fits into seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

/* static cache for the sequence values and for the intermediary coefficients c(k) */
static seq_t *a_cache = NULL;   /* a(n) values */
static size_t  a_cap   = 0;     /* allocated capacity for a_cache */
static size_t  a_used  = 0;     /* number of a values currently stored */

static seq_t *c_cache = NULL;   /* c(k) values */
static size_t  c_cap   = 0;     /* allocated capacity for c_cache */
static size_t  c_used  = 0;     /* number of c values currently stored */

seq_t a(idx_t n)
{
    /* initialise caches on first call */
    if (a_cache == NULL) {
        ensure_capacity(1, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[0] = (seq_t)-1;          /* a(0) = -1 */
        a_used = 1;
    }

    if (c_cache == NULL) {
        ensure_capacity(1, &c_cap, (void **)&c_cache, sizeof(seq_t));
        c_cache[0] = (seq_t)1;           /* c(0) = 1 */
        c_used = 1;
    }

    /* extend caches until we have a[n] */
    while (a_used <= (size_t)n) {
        idx_t m = (idx_t)a_used;          /* index of a to compute next */

        /* make sure c_cache has values up to k = m-1 */
        while (c_used <= (size_t)(m - 1)) {
            idx_t k = (idx_t)c_used;      /* next k to compute */
            seq_t prev = c_cache[k - 1];

            /* temp = prev * 3*(3k-4) / k  (exact division) */
            __int128 mult = (__int128)3 * ((__int128)3 * k - (__int128)4);
            __int128 temp  = (__int128)prev * mult;
            temp /= k;                    /* division is exact */

            seq_t val = (seq_t)temp;      /* cast back to seq_t (may truncate) */

            ensure_capacity(c_used + 1, &c_cap, (void **)&c_cache, sizeof(seq_t));
            c_cache[c_used] = val;
            ++c_used;
        }

        /* compute a[m] = Σ_{k=0}^{m-1} a[m-1-k] * c[k] */
        __int128 sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            sum += (__int128)a_cache[m - 1 - k] * (__int128)c_cache[k];
        }

        seq_t val = (seq_t)sum;           /* cast back to seq_t (may truncate) */

        ensure_capacity(a_used + 1, &a_cap, (void **)&a_cache, sizeof(seq_t));
        a_cache[a_used] = val;
        ++a_used;
    }

    return a_cache[n];
}

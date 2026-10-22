/*
 * A399578  Number of self-conjugate 2-dimensional vector partitions of (n, n)
 *          into strictly distinct parts.
 * https://oeis.org/A399578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 24, u32 n <= 61, u64 n <= 166, u128 n <= 460,
 *            big any n
 * Helpers:   compute_b_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   a(n) = (1/n) * Sum_{j=1}^{n} b(j) * a(n-j)
 *
 *   b(n) = Sum_{d|n} d * (1 + floor((d+1)/2)) * (-1)^(n/d - 1)
 *
 *   The recurrence is used to compute all values in ascending order,
 *   with memoisation for arbitrary order of calls.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_b_value.h"

static seq_t *a_cache = NULL;       /* a[0..] values */
static size_t a_cap  = 0;
static size_t a_size = 0;           /* number of computed a-values */

static int64_t *b_cache = NULL;     /* b[0..] values (int64_t for safety) */
static size_t b_cap  = 0;
static size_t b_size = 0;           /* number of computed b-values */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* ensure that a_cache and b_cache contain values up to index n */
static void ensure_cache_upto(idx_t n)
{
    /* resize a_cache if needed */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap   = new_cap;
    }

    /* resize b_cache if needed */
    if ((size_t)n + 1 > b_cap) {
        size_t new_cap = (size_t)n + 1;
        b_cache = (int64_t *)realloc(b_cache, new_cap * sizeof(int64_t));
        b_cap   = new_cap;
    }

    /* compute missing b(j) values */
    if (b_size <= (size_t)n) {
        for (idx_t j = b_size; j <= n; ++j) {
            if (j == 0)
                b_cache[0] = 0;          /* dummy, never used */
            else
                b_cache[j] = compute_b_value(j);
        }
        b_size = (size_t)n + 1;
    }

    /* initialize a(0) if not yet */
    if (a_size == 0) {
        a_cache[0] = (seq_t)1;
        a_size = 1;
    }

    /* compute a(k) for k = a_size .. n */
    for (idx_t k = a_size; k <= n; ++k) {
#if defined(__cplusplus)
        seq_t sum_pos = 0, sum_neg = 0;
#else
        __uint128_t sum_pos = 0, sum_neg = 0;
#endif
        for (idx_t j = 1; j <= k; ++j) {
            int64_t bj   = b_cache[j];
            uint64_t abs_bj = (bj >= 0) ? (uint64_t)bj : -(uint64_t)bj;
#if defined(__cplusplus)
            seq_t term = a_cache[k - j] * (seq_t)abs_bj;
            if (bj >= 0)
                sum_pos += term;
            else
                sum_neg += term;
#else
            __uint128_t prod = (__uint128_t)a_cache[k - j] * abs_bj;
            if (bj >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
#endif
        }
#if defined(__cplusplus)
        seq_t numerator = sum_pos - sum_neg;
        a_cache[k] = numerator / (seq_t)k;
#else
        __uint128_t numerator = sum_pos - sum_neg;
        a_cache[k] = (seq_t)(numerator / k);
#endif
    }
    a_size = (size_t)n + 1;
}

/* ------------------------------------------------------------ */
/* Public function as required.                                  */
seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return a_cache[n];
}

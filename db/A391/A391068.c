/*
 * A391068  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^j *
 *          binomial(k,j) * (2-j)^n.
 * https://oeis.org/A391068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 42, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   my_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391068: Triangle read by rows: T(n,k) = Σ_{j=0..k} (-1)^j C(k,j)(2-j)^n, flattened row-wise. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/my_binomial.h"

/* Static cached triangle values */
static idx_t      next_row = 0;    /* next row to generate */
static idx_t      cache_len = 0;   /* number of values stored */
static idx_t      cache_cap = 0;   /* current capacity of cache */
static seq_t   *cache = NULL;     /* locally cached values */

/* Compute base^exp using binary exponentiation. */
static seq_t my_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Compute T(r,k). */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t val = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin = my_binomial(k, j);
        seq_t base = (seq_t)2 - (seq_t)j;   /* careful signed subtraction */
        seq_t powv = my_pow(base, r);
        seq_t term = bin * powv;
        if (j & 1) val -= term;
        else       val += term;
    }
    return val;
}

/* Ensure cache contains all values up to index n. */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len <= n) {
        idx_t r = next_row;
        for (idx_t k = 0; k <= r; ++k) {
            seq_t val = compute_T(r, k);
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
        ++next_row;
    }
}

/* Public API: a(n) returns the n-th term (0‑based). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

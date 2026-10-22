/*
 * A377732  Numbers k such that max{d|k, d <= sqrt(k)} + min{d|k, d >=
 *          sqrt(k)} is a square.
 * https://oeis.org/A377732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1210, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   best_div_rec, factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that max{d | k, d ≤ √k} + min{d | k, d ≥ √k} is a perfect square. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities provided by the build environment. */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/best_div_rec.h"

/* Integer floor square root of an unsigned 64‑bit integer. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n)   lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* Test whether a positive integer k satisfies the sequence condition. */
static bool good_k(uint64_t k)
{
    if (k == 0) return false;

    uint64_t sqrt_k = floor_isqrt_u64((idx_t)k);

    /* Factorise k. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    /* Largest divisor ≤ sqrt(k). */
    uint64_t max_div = 1;
    best_div_rec(primes, exps, cnt, 0, 1, sqrt_k, &max_div);

    uint64_t other = k / max_div;

    /* Check if the sum is a perfect square. */
    uint64_t sum      = max_div + other;
    uint64_t sqrt_sum = floor_isqrt_u64((idx_t)sum);
    return sqrt_sum * sqrt_sum == sum;
}

/* Static state for memoisation. */
static size_t seq_len  = 0;          /* number of terms stored so far */
static size_t seq_cap  = 0;          /* allocated array capacity */
static seq_t *seq_vals = NULL;      /* array holding the sequence terms */
static idx_t  next_k   = 1;          /* next integer to test whether it belongs to the sequence */

/* Main sequence function as required by the harness. */
seq_t a(idx_t n)
{
    while (seq_len < (size_t)n) {
        if (good_k((uint64_t)next_k)) {
            if (seq_len >= seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
                seq_vals = seq_realloc(seq_vals, seq_cap, new_cap);
                seq_cap = new_cap;
            }
            seq_vals[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq_vals[(size_t)n - 1];
}

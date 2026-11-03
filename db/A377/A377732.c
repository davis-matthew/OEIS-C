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
 * Validated: standard, pass, 81 terms through n = 9899 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that max{d | k, d ≤ √k} + min{d | k, d ≥ √k} is a perfect square. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities provided by the build environment. */
#include "../utils/factor_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Recursively find the largest divisor of a factorised number not exceeding limit. */
static void best_div_rec(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         size_t idx, uint64_t cur, uint64_t limit,
                         uint64_t *best)
{
    if (idx == cnt) {
        if (cur <= limit && cur > *best) *best = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    uint64_t val = cur;
    for (size_t i = 0; i <= e; ++i) {
        best_div_rec(pr, ex, cnt, idx + 1, val, limit, best);
        if (i == e) break;
        if (val > limit / p) break;   /* prevent overflow */
        val *= p;
    }
}

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

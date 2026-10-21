/*
 * A258215  Number of steps from n to 0, where allowable steps are x -> [x/r]
 *          if x is in lower Wythoff sequence (A000201) and x -> [r*x]
 *          otherwise, where [ ] = floor and r = (1+sqrt(5))/2 = golden ratio.
 * https://oeis.org/A258215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A258215: Number of steps from n to 0 by
   x -> floor(x/φ) if x is in the lower Wythoff sequence
   (A000201), otherwise x -> floor(φ*x), where φ = (1+√5)/2. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values (floor) */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;          /* hi^2 > 2^64-1 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* floor(n * φ) */
static inline idx_t floor_nphi(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t prod = 5ULL * n * n;          /* 5*n*n fits in 64 bits for practical n */
    uint64_t r = isqrt64(prod);            /* floor(n * sqrt(5)) */
    return (n + r) >> 1;                  /* (n + floor(n√5)) / 2 */
}

/* floor(n / φ) */
static inline idx_t floor_inv_phi(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t prod = 5ULL * n * n;
    uint64_t r = isqrt64(prod);            /* floor(n * sqrt(5)) */
    return (r - n) >> 1;                  /* (floor(n√5) - n) / 2 */
}

/* test whether n belongs to the lower Wythoff sequence A000201 */
static inline bool is_lower(idx_t n)
{
    if (n == 0)
        return false;
    /* index k of the lower sequence for which floor(k*φ) may equal n */
    idx_t k = floor_inv_phi(n) + 1;
    uint64_t prod = 5ULL * k * k;
    uint64_t r = isqrt64(prod);            /* floor(k * sqrt(5)) */
    idx_t fp = (k + r) >> 1;               /* floor(k*φ) */
    return fp == n;
}

/* sequence value a(n) */
seq_t a(idx_t n)
{
    seq_t steps = 0;
    while (n > 0) {
        if (is_lower(n))
            n = floor_inv_phi(n);
        else
            n = floor_nphi(n);
        ++steps;
    }
    return steps;
}

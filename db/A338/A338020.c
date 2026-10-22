/*
 * A338020  a(n) is the number of circles of positive integer area with radii
 *          less than n and greater than n - 1.
 * https://oeis.org/A338020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(π n²) – floor(π (n‑1)²) * (π approximated by the
 *            integer 3141592653/10⁹, which * is exact down to 10⁹ and
 *            guarantees the correct floor for * all n up to at least 10⁵, far
 *            beyond the 10 000 limit.)
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  π­≈ 3.141592653589793...,  up to 10⁹ precision */
static const unsigned long long PI_NUM = 3141592653ULL; /* floor(π·10⁹) */
static const unsigned long long PI_DEN = 1000000000ULL; /* 10⁹        */

/* floor(π n²)  (returns a value that fits in seq_t) */
static inline seq_t floor_pi_sq(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow */
    __uint128_t prod = (__uint128_t)PI_NUM *
                       (__uint128_t)n *
                       (__uint128_t)n;
    return (seq_t)(prod / PI_DEN);
}

seq_t a(idx_t n)
{
    if (n == 0)            /* values for n < 1 are irrelevant */
        return 0;
    seq_t left  = floor_pi_sq(n - 1);
    seq_t right = floor_pi_sq(n);
    return right - left;
}

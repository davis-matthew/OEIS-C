/*
 * A257984  Nonhomogeneous Beatty sequence: ceiling((n - 1/2)*Pi).
 * https://oeis.org/A257984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((n - 1/2) * Pi)
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 0 for n == 0 (unused). */
    if (n == 0) return 0;

    /* Scale factors for high‑precision integer arithmetic.
       SCALE = 10^18  (fits in uint64_t), PI_SCALED = Pi * SCALE  (≈3.141592653589793238). */
    const uint64_t  SCALE      = 1000000000000000000ULL;          /* 1e18   */
    const uint64_t  PI_SCALED  = 3141592653589793238ULL;           /* Pi*1e18 in integer form */

    /* Compute floor((2n-1) * Pi / 2) using 128‑bit intermediate to avoid overflow. */
    __uint128_t two_n_minus_one = (__uint128_t)2 * n - 1;
    __uint128_t numerator       = two_n_minus_one * (__uint128_t)PI_SCALED;
    __uint128_t denominator     = (__uint128_t)2 * SCALE;
    __uint128_t floor_val       = numerator / denominator;

    /* a(n) = floor((2n-1)*Pi/2) + 1.  Cast back to seq_t (may be 8/16/32/64/128 bit
       unsigned integer or arbitrary‑precision class). */
    seq_t result = (seq_t)(floor_val + 1);
    return result;
}

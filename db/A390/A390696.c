/*
 * A390696  a(n) = floor(n * log_10(2^64)).
 * https://oeis.org/A390696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * log_10(2^64))
 * Range:     u8 n <= 13, u16 n <= 3401, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 10^10 is used as the scaling factor.  SCALE is floor(64*log10(2) * 10^10). */
static const uint64_t SCALE = 192659197224ULL;  /* 192,659,197,224  ≈ 19.2659197224 * 10^10 */
static const uint64_t DENOM = 10000000000ULL;   /* 10^10 */

/* Computes a(n) = floor(n * log_10(2^64)) for any non‑negative n.  */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow and preserve exactness. */
    __uint128_t prod = (__uint128_t)n * SCALE;
    return (seq_t)(prod / DENOM);
}

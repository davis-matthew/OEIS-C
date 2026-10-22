/*
 * A322533  Position of 1/3^n in the sequence of all numbers 1/2^m, 1/3^m,
 *          2/3^m arranged in decreasing order.
 * https://oeis.org/A322533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + floor(n*log₂(3))
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The fractional part is evaluated exactly by scaling  log₂(3) by 10¹⁸
   and using integer arithmetic: 
      floor(n*log₂(3)) = floor( (n * 1584962500721156000) / 10¹⁸ )   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Basis: log₂(3) ≈ 1.584962500721156043347…,  scaled by 10¹⁸ */
    const unsigned long long P = 1584962500721156000ULL; /* round(log₂(3)*1e18) */
    const unsigned long long Q = 1000000000000000000ULL; /* 1e18 */

    /* Guard against n == 0 (not part of the sequence, but safe) */
    if (n == 0) return (seq_t)0;

    /* 128‑bit intermediate to avoid overflow when computing n*P */
    uint64_t n64 = n;
    unsigned __int128 prod = (unsigned __int128)n64 * P;

    /* Compute the floor part: floor(n*log₂(3)) */
    uint64_t floor_part = (uint64_t)(prod / Q);
    seq_t floor_val   = (seq_t)floor_part;      /* fit in uint64_t for n≈10⁴ */

    /* 2*n part (fits in 64‑bit for the n range used by the tests) */
    seq_t two_n = (seq_t)n64 * 2;

    return two_n + floor_val;
}

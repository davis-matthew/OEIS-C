/*
 * A296357  a(n) = ceiling of n/Pi.
 * https://oeis.org/A296357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / pi)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^12 as scaling factor – fits in 64‑bit */
    const uint64_t SCALE = 1000000000000ULL;             /* 10^12 */
    /* ceil(pi * 10^12) */
    const uint64_t PI_SCALE = 3141592653590ULL;          /* 3.141592653589793…*10^12 rounded up */

    /* Use 128‑bit intermediate to avoid overflow on 64‑bit seq_t */
    __uint128_t prod = (__uint128_t)n * SCALE;
    uint64_t q = (uint64_t)(prod / PI_SCALE);
    uint64_t r = (uint64_t)(prod % PI_SCALE);

    /* Since PI_SCALE > pi*10^12, remainder is always >0, but keep test for safety */
    return r ? (seq_t)(q + 1) : (seq_t)q;
}

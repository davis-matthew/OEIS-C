/*
 * A327217  Positions of 0's in {A327216(n) : n > 0}.
 * https://oeis.org/A327217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer k with A327216(k) = 0, where
 *            A327216(k) = floor((2k+3)*phi) - floor((k+3)*phi) -
 *            floor(k*phi); computed by a linear search, with floor(k*phi) =
 *            (k + isqrt(5*k^2))/2 exactly
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9199 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

/* Exact floor(k*phi), phi = (1+sqrt(5))/2: floor((k + sqrt(5k^2))/2)
   = floor((k + floor(sqrt(5k^2)))/2) */
static uint64_t A327217_floor_phi(uint64_t k)
{
    return (k + isqrt_u64(5 * k * k)) / 2;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        uint64_t f0 = A327217_floor_phi(k);
        uint64_t f1 = A327217_floor_phi(k + 3);
        uint64_t f2 = A327217_floor_phi(2 * k + 3);
        if (f2 == f1 + f0) count++;
    }
    return (seq_t)k;
}

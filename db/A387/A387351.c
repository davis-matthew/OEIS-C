/*
 * A387351  Positions of -1's in A387348.
 * https://oeis.org/A387351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387351 – Positions of -1's in A387348
   Definition:  a(n) is the n-th positive integer k such that
                floor(k*φ) = floor(k*φ – φ/2),
               where φ = (1+√5)/2.  Equivalently,
               the fractional part of k*φ is at least φ/2. */
#include <stdint.h>
#include <math.h>

/* Integer square root for unsigned __int128.
   Returns the largest r such that r*r <= x. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    /* Approximate with long double sqrt, then adjust */
    uint64_t r = (uint64_t)sqrt((long double)x);
    while ((unsigned __int128)r * r > x) r--;
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (1) {
        k++;
        /* floor_k_phi = floor(k*φ) = (k + floor(k*√5)) / 2 */
        unsigned __int128 kk = (unsigned __int128)k;
        unsigned __int128 val5k = 5 * kk * kk;
        uint64_t sqrt5k = isqrt_uint128(val5k);
        idx_t floor_k_phi = (k + sqrt5k) / 2;

        /* t = 2*k - 1; floor_k_phi_minus = floor((t + t*√5)/4) */
        uint64_t t = 2 * k - 1;
        unsigned __int128 tt = (unsigned __int128)t;
        unsigned __int128 val5t = 5 * tt * tt;
        uint64_t sqrt5t = isqrt_uint128(val5t);
        idx_t floor_k_phi_minus = (t + sqrt5t) / 4;

        if (floor_k_phi_minus == floor_k_phi) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
    }
}

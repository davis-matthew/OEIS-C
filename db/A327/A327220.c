/*
 * A327220  Positions of 0's in {A327219(k) : n > 0}.
 * https://oeis.org/A327220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327219(k) = floor((2k+4)*phi) -
 *            floor((k+4)*phi) - floor(k*phi) = 0, phi=(1+sqrt(5))/2; found by
 *            a linear scan over k
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9686 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

typedef unsigned __int128 A327220_u128;

/* floor(sqrt(x)) for x < 2^126 by bitwise binary search */
static uint64_t A327220_isqrt(A327220_u128 x)
{
    uint64_t r = 0;
    int b;
    for (b = 62; b >= 0; b--) {
        uint64_t c = r | ((uint64_t)1 << b);
        if ((A327220_u128)c * c <= x)
            r = c;
    }
    return r;
}

/* floor(k*phi) = floor((k + floor(sqrt(5 k^2))) / 2) */
static uint64_t A327220_floor_kphi(uint64_t k)
{
    A327220_u128 t = (A327220_u128)k * k * 5;
    uint64_t r = A327220_isqrt(t);
    return (k + r) / 2;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        uint64_t v = A327220_floor_kphi(2 * k + 4)
                   - A327220_floor_kphi(k + 4)
                   - A327220_floor_kphi(k);
        if (v == 0)
            count++;
    }
    return (seq_t)k;
}

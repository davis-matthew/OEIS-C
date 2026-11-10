/*
 * A327254  Positions of 0's in {A327253(n) : n > 0}.
 * https://oeis.org/A327254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327253(k) = 0, where A327253(k) =
 *            floor(2*k*sqrt(6)) - 2*floor(k*sqrt(6)); computed by a linear
 *            scan (A327253(k) = 0 iff floor(sqrt(24*k^2)) is even)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9823 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A327254_isqrt(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;
    while (bit > x)
        bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (uint64_t)r;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k++;
        unsigned __int128 v = (unsigned __int128)24 * (unsigned __int128)k * (unsigned __int128)k;
        uint64_t f = A327254_isqrt(v);
        if ((f & 1) == 0)
            count++;
    }
    return (seq_t)k;
}

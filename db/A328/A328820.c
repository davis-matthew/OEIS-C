/*
 * A328820  Fourth digit after decimal point of square root of n.
 * https://oeis.org/A328820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (floor(sqrt(n * 100000000)) mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the fourth digit after the decimal point of sqrt(n). */

#include <stdint.h>

/* Integer square root of an unsigned __int128 using binary search. */
static unsigned __int128 isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = x;
    while (high > low + 1) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    const unsigned __int128 factor = 100000000ULL;   /* 10^8 */
    unsigned __int128 prod = (unsigned __int128)n * factor;
    unsigned __int128 root = isqrt_uint128(prod);
    return (seq_t)(root % 10);
}

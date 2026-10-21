/*
 * A284678  Positions of 0 in A284677; complement of A284679.
 * https://oeis.org/A284678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(n-1)*(5+√13)/2⌋ + 2 (OEIS notation)
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for 128‑bit unsigned values */
static uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = 1;
    /* find an upper bound */
    while ((__int128)high * high <= x) {
        high <<= 1;
    }
    /* binary search for floor(sqrt(x)) */
    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 mid_sq = (__int128)mid * mid;
        if (mid_sq <= x) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; behaviour for n == 0 is undefined */
    if (n == 0) return (seq_t)0;  /* guard against underflow */
    idx_t m = n - 1;               /* m = n-1 */

    /* compute floor(m * sqrt(13)) as floor(sqrt(13*m^2)) */
    unsigned __int128 prod = (unsigned __int128)13 * m * m;
    uint64_t fsqrt = isqrt128(prod);          /* floor(m * sqrt(13)) */

    /* floor((floor(m*sqrt13) - 3*m)/2)  == floor(m*α)  where α=(√13-3)/2 */
    uint64_t part = (fsqrt - 3 * m) / 2;

    /* a(n) = 4*n - 2 + part */
    seq_t result = (seq_t)n;
    result = result * 4;     /* 4*n */
    result = result - 2;     /* 4*n - 2 */
    result = result + (seq_t)part;   /* + part */
    return result;
}

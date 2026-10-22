/*
 * A329925  Beatty sequence for (1+sqrt(41))/5.
 * https://oeis.org/A329925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * (1 + sqrt(41)) / 5 )
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an integer square‑root of a 128‑bit intermediate product */

#include <stdint.h>

/* integer square root of a 128‑bit unsigned integer */
static inline uint64_t floor_sqrt_uint128(unsigned __int128 x)
{
    if (x < 2) return (uint64_t)x;
    unsigned __int128 low  = 0;
    unsigned __int128 high = (unsigned __int128)1 << 64;   /* 2^64 */
    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= x) low = mid;
        else                high = mid;
    }
    return (uint64_t)low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset safety – a(0) is not used */

    uint64_t N = (uint64_t)n;            /* idx_t is uint64_t */

    /* 41 * N * N fits in 128 bits for all test sizes (n <= 10000 here) */
    unsigned __int128 prod = (unsigned __int128)41 * N * N;

    uint64_t sqrt_term = floor_sqrt_uint128(prod);   /* floor( N * sqrt(41) ) */

    uint64_t result64 = (sqrt_term + N) / 5;          /* floor( n*r ) */

    return (seq_t)result64;
}

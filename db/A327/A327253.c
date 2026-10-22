/*
 * A327253  a(n) = floor(2*n*r) - 2*floor(n*r), where r = sqrt(6).
 * https://oeis.org/A327253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n*sqrt(6)) - 2*floor(n*sqrt(6))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute integer square root of an unsigned __int128 value.
   The returned value lies in the range [0, 2^64).  */
static seq_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;   /* highest power of 4 <= x */

    /* Find the largest power of 4 <= x          */
    while (bit > x)
        bit >>= 2;

    /* Main loop of the digit-by-digit method */
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (seq_t)r;
}

/* a(n) = floor(2*n*sqrt(6)) - 2*floor(n*sqrt(6))   */
seq_t a(idx_t n)
{
    /* nsq  = n^2 (128‑bit to avoid overflow)      */
    unsigned __int128 nsq = (unsigned __int128)n * (unsigned __int128)n;

    /* floor(n*sqrt(6))  = floor(sqrt(6*n^2))      */
    unsigned __int128 six_nsq   = (unsigned __int128)6 * nsq;
    seq_t floor_nr = isqrt_u128(six_nsq);

    /* floor(2*n*sqrt(6)) = floor(sqrt(24*n^2))   */
    unsigned __int128 twenty4_nsq = (unsigned __int128)24 * nsq;
    seq_t floor_2nr = isqrt_u128(twenty4_nsq);

    /* Result (always 0 or 1, fits into any seq_t) */
    return floor_2nr - (seq_t)2 * floor_nr;
}

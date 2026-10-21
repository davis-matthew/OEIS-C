/*
 * A277645  Beatty sequence for 3+sqrt(6).
 * https://oeis.org/A277645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(3+sqrt(6)))
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer (floor of sqrt). */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;          /* largest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long sq = nn * nn * 6ULL;      /* 6 * n^2  */
    unsigned long long root = isqrt_u64(sq);     /* floor(n*sqrt(6)) */
    return (seq_t)n * 3 + (seq_t)root;           /* floor(n*(3+sqrt(6))) */
}

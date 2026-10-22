/*
 * A351830  Distance from the n-th square pyramidal number (sum of the first n
 *          positive squares) to the nearest square.
 * https://oeis.org/A351830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = distance from the n-th square pyramidal number to the
 *            nearest square a(n) = | P(n) - nearest_square(P(n)) |, where
 *            P(n) = n(n+1)(2n+1)/6
 * Range:     u8 n <= 68, u16 n <= 2415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 128‑bit unsigned numbers */
static inline unsigned __int128 uint128_sqrt(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 p    = nn * (nn + 1) * (2 * nn + 1) / 6;   /* P(n) */
    unsigned __int128 s    = uint128_sqrt(p);                   /* floor(sqrt(P)) */
    unsigned __int128 diff1 = p - s * s;                        /* p - s²   */
    unsigned __int128 diff2 = (s + 1) * (s + 1) - p;             /* (s+1)² - p */
    return (seq_t)(diff1 <= diff2 ? diff1 : diff2);
}

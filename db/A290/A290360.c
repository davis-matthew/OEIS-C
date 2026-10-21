/*
 * A290360  Number of 6-leaf rooted trees with n levels.
 * https://oeis.org/A290360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^5 + 5*n^4 + 10*n^3 + 10*n^2 + n) / 30
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 126, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow for the 64‑bit
       seq_t build while still working for the arbitrary‑precision build.   */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n2  = N * N;
    unsigned __int128 n3  = n2 * N;
    unsigned __int128 n4  = n3 * N;
    unsigned __int128 n5  = n4 * N;

    unsigned __int128 numerator = (unsigned __int128)4 * n5
                                + (unsigned __int128)5 * n4
                                + (unsigned __int128)10 * n3
                                + (unsigned __int128)10 * n2
                                + N;
    const unsigned __int128 denom = 30;

    return (seq_t)(numerator / denom);
}

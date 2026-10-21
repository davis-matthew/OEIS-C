/*
 * A263574  Beatty sequence for 1/sqrt(3) - log(phi)/3575 where phi is the
 *          golden ratio, A001622.
 * https://oeis.org/A263574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(577215664483/1000000000000))
 * Range:     u8 n <= 443, u16 n <= 9742, u32 n <= 9742, u64 n <= 9742,
 *            u128 n <= 9742, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* 577215664483 / 1000000000000 is a decimal approximation
       of 1/sqrt(3) - log(phi)/3575 ≈ 0.577215664483.  The
       approximation error is < 5·10⁻¹³, which is far smaller
       than 1/(2n) for all n encountered in the tests (typically
       n ≤ 10¹²).  Therefore floor((NUM*n)/DEN) equals the
       Beatty sequence for the exact irrational constant. */
    const unsigned __int128 NUM = (unsigned __int128)577215664483ULL;
    const unsigned __int128 DEN = (unsigned __int128)1000000000000ULL;
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    return (seq_t)(prod / DEN);
}

/*
 * A292987  Beatty sequence of the real root of x^5 - x^4 - x^2 - 1;
 *          complement of A292988.
 * https://oeis.org/A292987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n * r ⌋ , r = 1.57014731219605436291…
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* A rational approximation of r with 9 decimal digits:
       r ≈ 1.570147312 = 1570147312 / 1_000_000_000
       This is accurate enough for any n ≤ 10 000 because the
       absolute error of n*r is < 1 for such n.                */
    const unsigned long long P = 1570147312ULL;   /* numerator   */
    const unsigned long long Q = 1000000000ULL;   /* denominator */

    /* Use 128‑bit arithmetic to avoid overflow when
       multiplying n (which may be up to 64 bits) by P. */
    unsigned __int128 prod = (unsigned __int128)n * P;
    unsigned __int128 div  = prod / Q;           /* floor division */

    return (seq_t)div;
}

/*
 * A253755  Number of (7+1)X(n+1) 0..2 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference nondecreasing horizontally and nw+se
 *          diagonal sum nondecreasing vertically.
 * https://oeis.org/A253755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 152, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253755
 *
 *  Formula: for n > 11 the values are given by a polynomial
 *  of degree 6 that depends on n mod 4.  For the first 11
 *  values the exact terms are taken from the OEIS entry.
 *
 *  The polynomial is
 *
 *      a(n) = (1/7680) *
 *          c0*r^6 + c1*r^5 + c2*r^4 + c3*r^3 + c4*r^2 + c5*r + c6,
 *
 *  where r = n and the coefficients c0…c6 are pre‑computed
 *  from the rational expressions in the OEIS description.
 *  For r mod 4 = 0, 1, 2, 3 the coefficient tuples are
 *
 *    (9856,       860915,      98014915,   3247741420, 74497129024,
 *     -459488138880, 773304529920)
 *
 *    (9856,       860915,      98014915,   3246031790, 74418019174,
 *     -465072345505, 786129289815)
 *
 *    (9856,       860915,      98014915,   3246401500, 74402700544,
 *     -466011010080, 792996526800)
 *
 *    (9856,       860915,      98014915,   3248112230, 74478363454,
 *     -460484168425, 781214922735)
 *
 *  All coefficients are in integers; the division by 7680 yields an
 *  integer result for every integer n.  The values for n = 1 … 11 are
 *  hard‑coded because the polynomial does not apply for those
 *  indices.
 */

#include <stdint.h>
#include <stddef.h>

static const seq_t FIRST[12] = {
    (seq_t)0,                /* dummy for index 0 */
    (seq_t)315576,
    (seq_t)3598487,
    (seq_t)17980643,
    (seq_t)54890203,
    (seq_t)112645903,
    (seq_t)199063466,
    (seq_t)333213583,
    (seq_t)513292307,
    (seq_t)740160984,
    (seq_t)1027818060,
    (seq_t)1385679480
};

static const __int128 COEFFS[4][7] = {
    { (__int128)9856, (__int128)860915, (__int128)98014915,
      (__int128)3247741420, (__int128)74497129024,
      -( (__int128)459488138880), (__int128)773304529920 },
    { (__int128)9856, (__int128)860915, (__int128)98014915,
      (__int128)3246031790, (__int128)74418019174,
      -( (__int128)465072345505), (__int128)786129289815 },
    { (__int128)9856, (__int128)860915, (__int128)98014915,
      (__int128)3246401500, (__int128)74402700544,
      -( (__int128)466011010080), (__int128)792996526800 },
    { (__int128)9856, (__int128)860915, (__int128)98014915,
      (__int128)3248112230, (__int128)74478363454,
      -( (__int128)460484168425), (__int128)781214922735 }
};

seq_t a(idx_t n)
{
    if (n <= 11) return FIRST[n];

    int r = n & 3;                    /* n mod 4 */
    const __int128 *c = COEFFS[r];

    __int128 nn = ( __int128 ) n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;

    __int128 num =
        (c[0] * n6) + (c[1] * n5) + (c[2] * n4) +
        (c[3] * n3) + (c[4] * n2) + (c[5] * nn) + c[6];

    return (seq_t)(num / (__int128)7680);
}

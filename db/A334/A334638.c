/*
 * A334638  Three-column array pPT read by rows: subsequence of primitive
 *          Pythagorean triples (x, y, z) with x = A153893^2 - A000079^2, y =
 *          2*A153893*A000079, z = A153893^2 + A000079^2, ordered by
 *          increasing z.
 * https://oeis.org/A334638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) reads the three-column array of primitive Pythagorean *
 *            triples: a(3*i) = x(i) = 2^(2*i+3) - 3*2^(i+1) + 1 * a(3*i+1) =
 *            y(i) = 2^(i+1)*(3*2^i - 1) * a(3*i+2) = z(i) = 10*2^(2*i) -
 *            6*2^i + 1 * The constants 10*2^(2*i) and 6*2^i are guaranteed to
 *            be ≤ the * subsequent 2^(2*i+3) term for all i...
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    idx_t     grp   = n / 3;          /* the triple index i */
    idx_t     rem   = n % 3;          /* 0 → x, 1 → y, 2 → z */

    seq_t pow2_grp_plus1 = pow2(grp + 1);   /* 2^(i+1) */
    seq_t pow2_grp       = pow2(grp);       /* 2^i         */
    seq_t pow2_2grp       = pow2(2 * grp);  /* 2^(2i)     */
    seq_t pow2_2grp_plus3 = pow2(2 * grp + 3); /* 2^(2i+3) */

    switch (rem)
    {
        case 0: { /* x(i) */
            seq_t sub = pow2_grp_plus1 * (seq_t)3;
            seq_t tmp = pow2_2grp_plus3 - sub; /* term > sub for all i */
            return tmp + (seq_t)1;
        }
        case 1: { /* y(i) */
            seq_t inner = pow2_grp * (seq_t)3 - (seq_t)1; /* 3*2^i - 1  */
            return pow2_grp_plus1 * inner;
        }
        case 2: { /* z(i) */
            seq_t term1 = pow2_2grp * (seq_t)10; /* 10*2^(2i) */
            seq_t term2 = pow2_grp   * (seq_t)6; /* 6*2^i     */
            seq_t tmp = term1 - term2;          /* term1 > term2 for all i */
            return tmp + (seq_t)1;
        }
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}

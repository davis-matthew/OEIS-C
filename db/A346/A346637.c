/*
 * A346637  a(n) is the number of quintuples (a_1,a_2,a_3,a_4,a_5) having all
 *          terms in {1,...,n} such that there exists a pentagon with these
 *          side-lengths.
 * https://oeis.org/A346637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - 5*binomial(n+1,5)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 85, u64 n <= 7192,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute n^5 using seq_t arithmetic to avoid overflow of idx_t */
    seq_t nn = (seq_t)n;
    seq_t n5 = nn * nn * nn * nn * nn;

    /* compute 5 * binomial(n+1,5) */
    seq_t bin = binomial(n + 1, 5);
    seq_t sub = (seq_t)5 * bin;

    return n5 - sub;
}

/*
 * A283026  Number of inequivalent 4 X 4 matrices with entries in {1,2,3,..,n}
 *          up to row permutations.
 * https://oeis.org/A283026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4*(n^4 + 1)*(n^4 + 2)*(n^4 + 3)/24 = binom(n^4 + 3, 4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n^4 fits inside idx_t for the n values tested ( ≤ 10000 ) */
    idx_t n4 = n * n * n * n;   /* n^4 */
    idx_t m  = n4 + 3;          /* n^4 + 3 */
    return binomial(m, 4);      /* C(m,4) = desired value */
}

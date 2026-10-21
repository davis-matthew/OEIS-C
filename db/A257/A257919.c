/*
 * A257919  The number of combinatorial equivalence classes of n-endomorphisms
 *          on a rank-3 semigroup.
 * https://oeis.org/A257919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(((3^(n+1)-3)/2)^3 +
 *            3*n*((3^(n+1)-3)/2)+2*((3^(n+1)-3)/2))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t n_seq = (seq_t)n;

    /* 3^(n+1) */
    seq_t pow3 = pow_seq((seq_t)3, (int)(n + 1));

    /* L = (3^(n+1) - 3) / 2 */
    seq_t L = (pow3 - (seq_t)3) / (seq_t)2;

    /* L^3 */
    seq_t term1 = pow_seq(L, 3);

    /* 3*n*L */
    seq_t term2 = (seq_t)3 * n_seq * L;

    /* 2*L */
    seq_t term3 = (seq_t)2 * L;

    /* sum terms */
    seq_t sum = term1 + term2 + term3;

    /* divide by 6 */
    seq_t result = sum / (seq_t)6;

    return result;
}

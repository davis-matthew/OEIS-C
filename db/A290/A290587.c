/*
 * A290587  Number of irredundant sets in the n-Andrásfai graph.
 * https://oeis.org/A290587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-45*(8 + 3*2^n) + (1772 + 405*2^n)*n - 1010*n^2 + 25*n^3
 *            + 50*n^4 + 3*n^5)/120 for n>3 with a(1)=3, a(2)=16, a(3)=81
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 19, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Directly return the first three known terms. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)81;

    /* Convert the index once. */
    seq_t n_seq = (seq_t)n;

    /* Powers of n. */
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2   * n_seq;          /* n^3 */
    seq_t n4 = n3   * n_seq;          /* n^4 */
    seq_t n5 = n4   * n_seq;          /* n^5 */

    /* 2^n as a seq_t. */
    seq_t pow2n = pow2((idx_t)n);

    /* Positive contributions. */
    seq_t pos = (seq_t)3 * n5
              + (seq_t)50 * n4
              + (seq_t)25 * n3
              + (seq_t)1772 * n_seq
              + (seq_t)405 * n_seq * pow2n;

    /* Negative contributions. */
    seq_t sub = (seq_t)360          /* 45*8 */
              + (seq_t)135 * pow2n   /* 45*3*2^n */
              + (seq_t)1010 * n2;    /* 1010*n^2 */

    /* Final numerator and division by 120. */
    seq_t num = pos - sub;
    return num / (seq_t)120;
}

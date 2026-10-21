/*
 * A250772  Number of (4+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*2^(n-1) + 4*n^2 + 26*n + 22 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Index as seq_t for type‑safe arithmetic */
    seq_t nseq = (seq_t)n;

    /* 2^(n-1) – big_pow produces the result in the seq_t type */
    seq_t pow_term = big_pow((seq_t)2, nseq - (seq_t)1);

    /* Compute the closed form */
    seq_t result = pow_term * (seq_t)20;          /* 20 * 2^(n-1) */
    result += ((seq_t)4 * nseq * nseq);           /* 4 * n^2   */
    result += ((seq_t)26 * nseq);                 /* 26 * n    */
    result += (seq_t)22;                          /* constant  */

    return result;
}

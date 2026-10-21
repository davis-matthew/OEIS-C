/*
 * A287992  Number of (undirected) paths in the prism graph Y_n.
 * https://oeis.org/A287992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^(n + 1) - 5*n - n^2 - 13)*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 (outside the OEIS offset), though not required for the problem */
    if (n == 0ULL) return (seq_t)0;

    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute 2^(n+1) in a type safe way using the provided big_pow helper. */
    idx_t exp = n + 1ULL;
    seq_t pow2 = big_pow((seq_t)2, (seq_t)exp);

    /* Compute the numerator: 5*pow2 - 5*n - n^2 - 13 */
    seq_t term = (seq_t)5 * pow2;
    term -= (seq_t)5 * n_seq;
    term -= n_seq * n_seq;
    term -= (seq_t)13;

    /* Multiply by n to get the final value */
    term *= n_seq;
    return term;
}

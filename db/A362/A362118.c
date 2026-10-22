/*
 * A362118  a(n) = (10^(n*(n+1)/2)-1)/9.
 * https://oeis.org/A362118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10^(n*(n+1)/2)-1)/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes the n-th term of OEIS sequence A362118.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 we return 0. */
    if (n == 0) {
        return 0;
    }

    /* Compute t = n*(n+1)/2 as a seq_t to avoid overflow. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Compute 10^t.  */
    seq_t pow10t = big_pow((seq_t)10, t);

    /* Compute (10^t - 1) / 9, which is the repunit of t ones. */
    seq_t result = (pow10t - 1) / 9;

    return result;
}

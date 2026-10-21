/*
 * A275854  Number of labeled directed graphs on n nodes (allowing self loops)
 *          such that the out-degree of each node is at most 2.
 * https://oeis.org/A275854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + n + n*(n-1)/2)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = ((n^2 + n + 2)/2)^n        */

#include "../utils/big_pow.h"

/*  Computes the n‑th term of OEIS sequence A275854:
 *  Number of labeled directed graphs on n nodes (allowing self loops)
 *  such that the out-degree of each node is at most 2.
 *
 *  The value is given by:  a(n) = ( Σ_{j=0}^{2} binomial(n,j) )^n
 *                          = (1 + n + n*(n-1)/2)^n
 */
seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type used for the sequence */
    seq_t n_seq = (seq_t)n;

    /* Base case: a(0) = 1 */
    if (n_seq == (seq_t)0)
        return (seq_t)1;

    /* Compute the base: 1 + n + n*(n-1)/2   */
    seq_t base = n_seq + (seq_t)1;
    seq_t temp = n_seq * (n_seq - (seq_t)1);
    temp /= (seq_t)2;
    base += temp;

    /* Raise the base to the power n using a generic big‑integer exponentiation */
    return big_pow(base, n_seq);
}

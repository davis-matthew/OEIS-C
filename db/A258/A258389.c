/*
 * A258389  a(n) = (n^(n+1)-(n-1)^n) + ((n+1)^n-n^(n-1)).
 * https://oeis.org/A258389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n+1)-(n-1)^n) + ((n+1)^n-n^(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but keep safe */

    seq_t nseq   = (seq_t)n;
    seq_t n_minus_1 = nseq - 1;
    seq_t n_plus_1  = nseq + 1;

    /* Compute the required powers */
    seq_t pow_n_minus1_n = big_pow(n_minus_1, (seq_t)n);        /* (n-1)^n          */
    seq_t pow_n_inv1     = big_pow(nseq,       (seq_t)(n-1));   /* n^(n-1)          */
    seq_t pow_np1_n      = big_pow(n_plus_1,   (seq_t)n);        /* (n+1)^n          */
    seq_t pow_n_n1       = pow_n_inv1 * nseq * nseq;            /* n^(n+1) = n^(n-1)*n^2 */

    /* Combine according to the formula */
    seq_t term1 = pow_n_n1 - pow_n_minus1_n;
    seq_t term2 = pow_np1_n - pow_n_inv1;

    return term1 + term2;
}

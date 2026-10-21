/*
 * A246923  Expansion of g.f.: 1 / AGM(1-9*x, sqrt((1-x)*(1-81*x))).
 * https://oeis.org/A246923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( Σ_{k=0..n} 2^k * C(n,k) * C(2k,k) )^2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the n-th term of OEIS sequence A246923.
   seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Use iterative multiplication to avoid heavy factorial or
       binomial calls.  The following variables are kept in seq_t
       to allow arbitrary‑precision arithmetic.  */
    seq_t sum = (seq_t)1;           /* S(0) = 1  */
    seq_t binom_nk = (seq_t)1;      /* C(n,0) */
    seq_t binom_2k_k = (seq_t)1;    /* C(0,0) */
    seq_t pow2k = (seq_t)1;         /* 2^0 */

    /* For k = 0 we already added term = 1*1*1 = 1. */
    for (idx_t k = 0; k < n; ++k)
    {
        /* Prepare next coefficient values: C(n,k+1), C(2(k+1),k+1),
           and 2^(k+1).  All divisions are exact integers.  */
        /* C(n,k+1) = C(n,k) * (n-k) / (k+1)   */
        seq_t num_nk = (seq_t)(n - k);
        seq_t den_nk = (seq_t)(k + 1);
        binom_nk = binom_nk * num_nk / den_nk;

        /* C(2(k+1),k+1) = C(2k, k) * [2*(k+1)*(2*(k+1)-1)] / [(k+1)^2]  */
        seq_t num_2k = (seq_t)(2 * (k + 1));
        seq_t den_2k = (seq_t)(k + 1);
        binom_2k_k = binom_2k_k * (num_2k * (num_2k - 1)) / (den_2k * den_2k);

        /* 2^(k+1) = 2^k * 2   */
        pow2k = pow2k * (seq_t)2;

        /* Add term for the new k+1 */
        seq_t term = binom_nk * binom_2k_k;
        term = term * pow2k;
        sum += term;
    }

    /* Return the square of the sum: a(n) = S(n)^2  */
    return sum * sum;
}

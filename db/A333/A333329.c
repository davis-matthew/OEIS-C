/*
 * A333329  Number of winnable configurations in Lights Out game (played on a
 *          digraph) summed over every labeled digraph on n nodes.
 * https://oeis.org/A333329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A286331(n,k) * 2^k. Where A286331(n,k) is
 *            the number of n×n binary matrices of rank k over GF(2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0) return (seq_t)1;

    /* 2^n, 2^k, and 2^{n-k} */
    seq_t pow2_n = pow2(n);     /* 2^n */
    seq_t pow2_k = (seq_t)1;    /* 2^k, initially 1 */
    seq_t pow2_n_minus_k = pow2_n; /* 2^{n-k}, initially 2^n */

    /* term_0 = 2^{0 + 0*(0-1)/2} * (product empty)^2 / 1 = 1 */
    seq_t term = (seq_t)1;
    seq_t sum  = term;          /* a(0) = 1 */

    for (idx_t k = 0; k < n; ++k) {
        /* numerator = 2^{k+1} * (2^{n-k} - 1)^2 */
        seq_t two_to_k_plus_1 = pow2_k * ((seq_t)2);           /* 2^{k+1} */
        seq_t diff = pow2_n_minus_k - ((seq_t)1);             /* 2^{n-k} - 1 */
        seq_t numerator = two_to_k_plus_1 * diff * diff;      /* 2^{k+1} * (2^{n-k} - 1)^2 */

        /* denominator = 2^{k+1} - 1 */
        seq_t denominator = two_to_k_plus_1 - ((seq_t)1);

        /* term_{k+1} = term_k * numerator / denominator */
        term = term * numerator;
        term = term / denominator;   /* exact division */

        sum += term;

        /* Update powers for next iteration */
        pow2_n_minus_k = pow2_n_minus_k / ((seq_t)2); /* 2^{n-(k+1)} */
        pow2_k = pow2_k * ((seq_t)2);                /* 2^{k+1} */
    }

    return sum;
}

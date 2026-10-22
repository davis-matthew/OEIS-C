/*
 * A334724  Denominator of the sum of all elements of the n X n matrix M with
 *          M[i,j] = (-1)^(i+j)*i/j for i,j = 1..n.
 * https://oeis.org/A334724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the sum of all elements of the n x n
 *            matrix M with M[i,j] = (-1)^(i+j)*i/j for i,j = 1..n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute H_alt = sum_{j=1..n} (-1)^j / j as a reduced fraction N/D. */
    seq_t N = 0;          /* absolute value of numerator (unsigned) */
    seq_t D = 1;          /* denominator (positive) */
    bool neg = false;     /* sign of the current fraction, irrelevant for N's abs */

    for (idx_t j = 1; j <= n; ++j) {
        /* term = (-1)^j * 1/j  -> numerator is 1, denom j, sign depends on j */
        bool term_neg = (j & 1);          /* j odd -> negative */
        seq_t term_num = 1;
        seq_t term_den = (seq_t)j;

        /* Add current term to N/D */
        seq_t prod1 = N * term_den;          /* N * j */
        seq_t prod2 = term_num * D;           /* 1 * D = D */
        bool prod1_neg = neg;
        bool prod2_neg = term_neg;

        seq_t new_num;
        bool new_neg;
        if (prod1_neg == prod2_neg) {
            new_num = prod1 + prod2;
            new_neg = prod1_neg;
        } else {
            if (prod1 >= prod2) {
                new_num = prod1 - prod2;
                new_neg = prod1_neg;
            } else {
                new_num = prod2 - prod1;
                new_neg = prod2_neg;
            }
        }
        seq_t new_den = D * term_den;

        seq_t g = gcd(new_num, new_den);
        N = new_num / g;
        D = new_den / g;
        neg = new_neg;
    }

    /* A = |(-1)^(n+1) * n/2| = |(n even)? n/2 : (n+1)/2| */
    seq_t A = (n & 1) ? ((seq_t)(n + 1) / (seq_t)2) : ((seq_t)n / (seq_t)2);

    /* Simplify denominator:
       denom = D / gcd(A * N, D)
       Use gcd properties to avoid overflow:
          gA = gcd(A, D)
          D1 = D / gA
          denom = D1 / gcd(N, D1)
    */
    seq_t gA = gcd(A, D);
    seq_t D1 = D / gA;
    seq_t gN = gcd(N, D1);
    seq_t denom = D1 / gN;

    return denom;
}

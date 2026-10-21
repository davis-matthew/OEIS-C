/*
 * A290343  Numerators of the Kirchhoff (and Harary) index for the n-hypercube
 *          graph.
 * https://oeis.org/A290343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Horary index numerator of the n‑hypercube graph
 *
 *  The Harary index for the n‑hypercube Q_n is
 *
 *      H(Q_n) = 2^{n-1} * sum_{k=1}^{n} C(n,k) / k
 *
 *  Its numerator a(n) is returned by this function.
 *
 *  The following recurrence in exact arithmetic is used:
 *
 *      H_0 = 0
 *      H_n = 2 * H_{n-1} + (2^{n-1} * (2^n - 1)) / n
 *
 *  with H_n written as a reduced fraction P/Q.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base case: n = 0 */
    if (n == 0)
        return 0;

    /* Fraction representing H_{n-1} = P/Q */
    seq_t P = 0;          /* numerator   */
    seq_t Q = 1;          /* denominator */

    seq_t pow2 = 1;       /* pow2 = 2^i-1 for current i   */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* 2^{i-1}  and  2^i  */
        seq_t pow2_i_minus1 = pow2;      /* 2^{i-1}           */
        seq_t pow2_i        = pow2 + pow2;   /* 2^i = 2 * 2^{i-1} */

        /* A_i = 2^{i-1} * (2^i - 1)  */
        seq_t A = pow2_i_minus1 * (pow2_i - 1);

        /* New numerator and denominator before reduction */
        seq_t new_num = 2 * P * (seq_t)i + A * Q;
        seq_t new_den = Q * (seq_t)i;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        P = new_num / g;
        Q = new_den / g;

        /* Prepare for next iteration */
        pow2 = pow2_i;
    }

    /* Return the numerator of the reduced fraction H_n */
    return P;
}

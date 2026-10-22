/*
 * A396151  a(n) = 2^(n+1) + 1 - 2*Re((1+i)^n), where i is the imaginary unit.
 * https://oeis.org/A396151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 1 - 2*Re((1+i)^n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute the nth term of OEIS sequence A396151.
 *   a(n) = 2^(n+1) + 1 - 2*Re((1+i)^n)
 *   where Re((1+i)^n) is the real part of (1+i)^n.
 */
seq_t a(idx_t n)
{
    /* Compute a_n = Re((1+i)^n) using the recurrence
     *   a_{k+1} = a_k - b_k
     *   b_{k+1} = a_k + b_k
     *   (a_0,b_0) = (1,0)
     */
    seq_t a_real = 1; /* real part */
    seq_t imag   = 0; /* imaginary part */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t next_real = a_real - imag;
        seq_t next_imag = a_real + imag;
        a_real = next_real;
        imag   = next_imag;
    }

    /* Compute 2^(n+1) */
    seq_t pow_term = pow2(n + 1);
    pow_term = pow_term + 1;           /* +1 */
    pow_term = pow_term - (a_real << 1);/* - 2*a_real */

    return pow_term;
}

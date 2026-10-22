/*
 * A397407  The number of 2-colorings of the Aztec diamond of order n up to
 *          rotation and reflection.
 * https://oeis.org/A397407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   When n is even, a(n) = 1/8*(2^(2*n*(n+1)) + 2*2^(n*(n+1)/2) +
 *            3*2^(n*(n+1)) + 2*2^(n*(3+2*n)/2)). When n is odd, a(n) =
 *            1/8*(2^(2*n*(n+1)) + 2*2^(n*(n+1)/2) + 3*2^(n*(n+1)) +
 *            2*2^((2*n+1)*(n+1)/2))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the distinct exponents */
    idx_t exp1 = 2 * n * (n + 1);            /* 2 * n * (n+1)                 */
    idx_t exp2 = n * (n + 1) / 2;            /* n * (n+1) / 2                 */
    idx_t exp3 = n * (n + 1);                /* n * (n+1)                     */
    idx_t exp4;

    if ((n & 1) == 0)                       /* n even */
        exp4 = n * (3 + 2 * n) / 2;
    else                                    /* n odd  */
        exp4 = (2 * n + 1) * (n + 1) / 2;

    /* Build the powers of two */
    seq_t p1 = pow2(exp1);
    seq_t p2 = pow2(exp2);
    seq_t p3 = pow2(exp3);
    seq_t p4 = pow2(exp4);

    /* Combine according to the formula */
    seq_t sum = p1 + (p2 * 2) + (p3 * 3) + (p4 * 2);

    return sum / 8;
}

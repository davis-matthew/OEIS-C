/*
 * A346642  a(n) = Sum_{j=1..n} Sum_{i=1..j} j^3*i^3.
 * https://oeis.org/A346642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(21*n^5+69*n^4+45*n^3-21*n^2-6*n+4)/672
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Promote the index to seq_t early. */
    seq_t nn = (seq_t)n;
    seq_t n1  = nn + (seq_t)1;
    seq_t n2  = nn + (seq_t)2;

    /* Compute powers of n. */
    seq_t n2t = nn * nn;        /* n^2 */
    seq_t n3t = n2t * nn;       /* n^3 */
    seq_t n4t = n3t * nn;       /* n^4 */
    seq_t n5t = n4t * nn;       /* n^5 */

    /* Polynomial numerator. */
    seq_t P = (seq_t)21 * n5t
            + (seq_t)69 * n4t
            + (seq_t)45 * n3t
            - (seq_t)21 * n2t
            - (seq_t) 6 * nn
            + (seq_t)4;

    /* Multiply all factors and divide by 672. */
    seq_t prod = nn * n1;
    prod *= n2;
    prod *= P;
    return prod / (seq_t)672;
}

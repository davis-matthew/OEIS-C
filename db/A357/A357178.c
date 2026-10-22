/*
 * A357178  First differences of cubes of triangular numbers.
 * https://oeis.org/A357178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 3*n^5)/4
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 89, u64 n <= 7553,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute n^2, n^3, and the factor (1 + 3*n^2) */
    seq_t n2 = N * N;                     /* n^2 */
    seq_t n3 = n2 * N;                    /* n^3 */
    seq_t term = (seq_t)1 + (seq_t)3 * n2; /* 1 + 3 n^2 */

    /* Divide the product n3*term by 4 without overflow.
       We perform two consecutive 2‑fold reductions, always
       dividing the factor that is even. */
    seq_t a1 = n3;
    seq_t t1 = term;

    /* First division by 2 */
    if ((a1 & 1) == 0)
        a1 >>= 1;
    else
        t1 >>= 1;

    /* Second division by 2 */
    if ((a1 & 1) == 0)
        a1 >>= 1;
    else
        t1 >>= 1;

    /* Final product fits into seq_t for all n where a(n) fits into seq_t. */
    return a1 * t1;
}

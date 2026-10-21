/*
 * A281999  Half of the height of the right trapezoidal gnomon (of the
 *          derivative of Y=X^5).
 * https://oeis.org/A281999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*(n^2 - 1)*n^2 - (-1)^n + 1)/2
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 203, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by definition of the sequence offset */
    uint64_t n64 = (uint64_t)n;
    uint64_t n2  = n64 * n64;              /* n^2 */
    uint64_t term1 = 5 * (n2 - 1) * n2;    /* 5 * (n^2-1) * n^2 */
    uint64_t result;

    if (n & 1) /* odd n: add 2 before division */
        result = (term1 + 2) / 2;
    else        /* even n: straightforward division */
        result = term1 / 2;

    return (seq_t)result;
}

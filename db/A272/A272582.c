/*
 * A272582  The number of strongly connected digraphs with n vertices and n+1
 *          edges.
 * https://oeis.org/A272582
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n+3)*n! / 4 (OEIS: A272582)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for n >= 2.  For n < 2 the result is 0. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute n!  */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* (n-2)*(n+3)  */
    seq_t mult = (seq_t)(n - 2) * (seq_t)(n + 3);

    if (n >= 4) {
        /* n! is divisible by 4 for n >= 4, so divide first to keep the
           intermediate smaller.  */
        fact = fact / 4;
        return fact * mult;
    } else {
        /* For n = 2 or 3 the division by 4 is applied after the product
           to avoid an inexact division.  */
        return (fact * mult) / 4;
    }
}

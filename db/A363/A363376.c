/*
 * A363376  Determinant of the n X n matrix formed by placing 1..n^2 in
 *          L-shaped gnomons in alternating directions.
 * https://oeis.org/A363376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1)*2^(n-3)*(2*n*(n-1)+1)*(n!)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* a(1) = 1 */

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* n! is even for n >= 2, so integer division by 2 is safe */
    seq_t half_fact = fact / 2;

    /* Compute 2^(n-2) via repeated doubling */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 *= (seq_t)2;

    /* Coefficient (2*n*(n-1)+1) */
    seq_t coeff = 2 * (seq_t)n * ( (seq_t)n - 1 ) + 1;

    /* Result before sign */
    seq_t result = coeff * half_fact * pow2;

    /* Apply (-1)^(n-1) */
    if ((n - 1) & 1)
        result = -result;

    return result;
}

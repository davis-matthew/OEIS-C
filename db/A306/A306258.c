/*
 * A306258  a(n) = floor(n^2/4)*n!.
 * https://oeis.org/A306258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^2/4)*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute n! using the provided helper */
    seq_t fact = factorial(n);

    /* Compute floor(n^2/4) in seq_t arithmetic */
    seq_t square = ((seq_t)n * (seq_t)n) / (seq_t)4;

    /* Return the product */
    return fact * square;
}

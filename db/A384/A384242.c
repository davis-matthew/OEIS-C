/*
 * A384242  a(n) = Product_{k=0..n-1} (4*n-5*k).
 * https://oeis.org/A384242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=0..n-1} (4*n-5*k)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0   (a(0) = 1) */

#include <stdint.h>  /* just in case seq_t or idx_t not defined elsewhere */

seq_t a(idx_t n)
{
    /* Special case for n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* If n is a positive multiple of 5, the product contains a zero factor */
    if (n % 5 == 0)
        return (seq_t)0;

    seq_t res = (seq_t)1;

    /* Compute product (4*n - 5*k) for k = 0 .. n-1 */
    for (idx_t k = 0; k < n; ++k)
    {
        /* All calculations are performed in seq_t to avoid truncation */
        seq_t term = (seq_t)n * (seq_t)4 - (seq_t)k * (seq_t)5;
        res = res * term;
    }

    return res;
}

/*
 * A266957  Numbers m such that 9*m+10 is a square.
 * https://oeis.org/A266957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (18*(n-1)*n + 5*(2*n-1)*(-1)^n - 3)/8
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (-1)^n: +1 for even n, -1 for odd n */
    seq_t sign = (n & 1) ? -1 : 1;

    /* Convert to seq_t early to avoid unsigned underflow */
    seq_t nn = (seq_t)n;
    seq_t term = ((seq_t)18 * (nn - 1) * nn
                  + (seq_t)5 * ((seq_t)(2 * nn - 1)) * sign
                  - (seq_t)3) / (seq_t)8;
    return term;
}

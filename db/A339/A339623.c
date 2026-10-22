/*
 * A339623  Consider a square drawn on the perimeter of a square lattice with
 *          side length n. a(n) is the number of regions inside the square
 *          after drawing unit circles centered at each interior lattice point
 *          of the square.
 * https://oeis.org/A339623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 - 18*n + 12 for n >= 3 a(1) = 1 a(2) = 5
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t tn = (seq_t)n;
    /* compute 7*n*n - 18*n + 12 safely in seq_t */
    return (seq_t)7 * tn * tn - (seq_t)18 * tn + (seq_t)12;
}

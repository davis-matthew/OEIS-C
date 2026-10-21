/*
 * A277636  Number of 3 X 3 matrices having all elements in {0,...,n} with
 *          determinant = permanent.
 * https://oeis.org/A277636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + 3*n + 1)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 22, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t t = (3 * m * m) + (3 * m) + 1;
    return t * t * t;
}

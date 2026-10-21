/*
 * A280344  Number of 2 X 2 matrices with all elements in {0,...,n} with
 *          determinant = permanent^n.
 * https://oeis.org/A280344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n == 0; 4*n^2 + 6*n + 2 for n > 0
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 995, u64 n <= 995,
 *            u128 n <= 995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t ns = (seq_t)n;
    return 4 * ns * ns + 6 * ns + (seq_t)2;
}

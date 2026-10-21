/*
 * A265081  Row 4 of array in A265080.
 * https://oeis.org/A265081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 6*n^3 - 7*n^2 + 4*n
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;          /* n² */
    seq_t n3 = n2 * x;         /* n³ */
    seq_t n4 = n2 * n2;        /* n⁴ */
    return n4 + 6 * n3 - 7 * n2 + 4 * x;
}

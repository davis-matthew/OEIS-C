/*
 * A256647  27-gonal pyramidal numbers: a(n) = n*(n+1)*(25*n-22)/6.
 * https://oeis.org/A256647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(25*n-22)/6
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 578, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    /* Exact division by 6 because the product is always divisible by 6 */
    return N * (N + 1) * (25 * N - 22) / 6;
}

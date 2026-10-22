/*
 * A374489  a(n) = floor(Sum_{k=n^4..(n+1)^4} k^(1/4)).
 * https://oeis.org/A374489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 + 8*n^3 + 8*n^2 + 5*n + 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    /* Compute the polynomial in a single expression to avoid intermediate overflows
       (the type seq_t handles the necessary size). */
    return 4 * N * N * N * N + 8 * N * N * N + 8 * N * N + 5 * N + 1;
}

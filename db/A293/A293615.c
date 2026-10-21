/*
 * A293615  a(n) = Pochhammer(n, 5) / 2.
 * https://oeis.org/A293615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (seq_t)n * (n+1) * (n+2) * (n+3) * (n+4) / 2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 94, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the product is 0; the formula naturally gives 0. */
    seq_t result = (seq_t)n * (n + 1) * (n + 2) * (n + 3) * (n + 4);
    result /= 2;
    return result;
}

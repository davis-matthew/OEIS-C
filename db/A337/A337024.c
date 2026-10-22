/*
 * A337024  Number of ways to tile a 2n X 2n square with 1 X 1 white and n X n
 *          black squares.
 * https://oeis.org/A337024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2 + 10*n + 3
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 the value is not used. */
    if (n == 0) return (seq_t)0;
    seq_t x = (seq_t)n;
    return (seq_t)3 * x * x + (seq_t)10 * x + (seq_t)3;
}

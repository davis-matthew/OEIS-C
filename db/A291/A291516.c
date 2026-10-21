/*
 * A291516  The arithmetic function uhat(n,3,7), negated.
 * https://oeis.org/A291516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27 for 1 ≤ n < 10, otherwise a(n) = 3*n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return 27;
    return (seq_t)n * 3;
}

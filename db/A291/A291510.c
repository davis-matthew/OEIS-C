/*
 * A291510  The arithmetic function uhat(n,2,5), negated.
 * https://oeis.org/A291510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14 if n <= 7, otherwise a(n) = 2*n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
        return (seq_t)14;
    return (seq_t)n * (seq_t)2;
}

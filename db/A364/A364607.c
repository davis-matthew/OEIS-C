/*
 * A364607  Denominations of a 4-coin system that returns the fewest coins in
 *          change on average.
 * https://oeis.org/A364607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   a(n) = {1,5,18,25} for n=1..4; zero otherwise
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)5;
    if (n == (idx_t)3) return (seq_t)18;
    if (n == (idx_t)4) return (seq_t)25;
    return (seq_t)0;
}

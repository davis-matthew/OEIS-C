/*
 * A254029  Positive solutions of Monkey and Coconuts Problem for the classic
 *          case (5 sailors, 1 coconut to the monkey): a(n) = 15625*n - 4 for
 *          n >= 1.
 * https://oeis.org/A254029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15625*n - 4
 * Range:     u8 none, u16 n <= 4, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 15625u - 4u;
}

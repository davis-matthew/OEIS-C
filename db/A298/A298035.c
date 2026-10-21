/*
 * A298035  Coordination sequence of Dual(3.12.12) tiling with respect to a
 *          trivalent node.
 * https://oeis.org/A298035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 18*n − 15
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return ((seq_t)n * (seq_t)18) - (seq_t)15;
}

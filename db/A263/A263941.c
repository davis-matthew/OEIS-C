/*
 * A263941  Minimal most likely sum for a roll of n 8-sided dice.
 * https://oeis.org/A263941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(9*n/2) for n>1, a(1) = 1
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    return ((seq_t)n * (seq_t)9) / (seq_t)2;
}

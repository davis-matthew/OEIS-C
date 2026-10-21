/*
 * A256680  Minimal most likely sum for a roll of n 4-sided dice.
 * https://oeis.org/A256680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(5*n/2) for n>=2; a(0)=0, a(1)=1
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    return ((seq_t)5 * (seq_t)n) / (seq_t)2;
}

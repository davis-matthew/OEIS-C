/*
 * A361871  The smallest order of a non-abelian group with an element of order
 *          n.
 * https://oeis.org/A361871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6, a(2)=6, a(n)=2*n for n>=3
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)6;
    return (seq_t)n * (seq_t)2;
}

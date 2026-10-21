/*
 * A253472  Square Pairs: Numbers n such that 1, 2, ..., 2n can be partitioned
 *          into n pairs, where each pair adds up to a perfect square.
 * https://oeis.org/A253472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4; a(2)=7; a(3)=8; a(4)=9; a(n)=n+7 for n>4
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)9;
    return (seq_t)n + (seq_t)7;
}

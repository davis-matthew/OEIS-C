/*
 * A347325  Solution to the spectator-first Tantalizer problem.
 * https://oeis.org/A347325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (⌊n/2⌋ + 1 - a(⌊n/2⌋)), a(1) = 1
 * Range:     u8 n <= 425, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    idx_t k = n >> 1;
    seq_t ak = a(k);
    seq_t res = (seq_t)k + 1 - ak;
    res *= 2;
    return res;
}

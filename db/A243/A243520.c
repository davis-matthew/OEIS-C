/*
 * A243520  Numbers that are congruent to {0, 8} mod 11.
 * https://oeis.org/A243520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * floor(n/2) + 8 * (n mod 2)
 * Range:     u8 n <= 46, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half = n >> 1;
    if ((n & 1) == 0)
        return (seq_t)half * 11;
    else
        return (seq_t)half * 11 + 8;
}

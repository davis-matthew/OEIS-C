/*
 * A259755  Numbers that are congruent to {4, 20} mod 24.
 * https://oeis.org/A259755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(6*n + (-1)^n - 3)
 * Range:     u8 n <= 21, u16 n <= 5461, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = (n - 1) / 2;
    if (n & 1U)
        return (seq_t)(4 + 24 * m);
    else
        return (seq_t)(20 + 24 * m);
}

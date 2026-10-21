/*
 * A256496  a(n) = 15(n mod 2) + 10(n mod 3) + 6(n mod 5).
 * https://oeis.org/A256496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*(n mod 2) + 10*(n mod 3) + 6*(n mod 5)
 * Range:     u8 n <= 282, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t r2 = t % (seq_t)2;
    seq_t r3 = t % (seq_t)3;
    seq_t r5 = t % (seq_t)5;
    return (seq_t)15 * r2 + (seq_t)10 * r3 + (seq_t)6 * r5;
}

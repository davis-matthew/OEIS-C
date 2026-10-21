/*
 * A255680  a(n) = n*(n mod 3)*(n mod 5).
 * https://oeis.org/A255680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n mod 3)*(n mod 5)
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t r = sn * (sn % (seq_t)3) * (sn % (seq_t)5);
    return r;
}

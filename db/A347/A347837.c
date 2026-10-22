/*
 * A347837  a(n) = 32*(n + floor(n/3)) - 11; third column of A347834.
 * https://oeis.org/A347837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*(n + floor(n/3)) - 11
 * Range:     u8 n <= 6, u16 n <= 1536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    seq_t third = v / 3;
    seq_t sum = v + third;
    seq_t val = sum * 32 - 11;
    return val;
}

/*
 * A347836  a(n) = 8*(n + floor(n/3)) - 3; second column of A347834.
 * https://oeis.org/A347836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n + floor(n/3)) - 3
 * Range:     u8 n <= 24, u16 n <= 6144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tmp = (seq_t)n + (seq_t)(n/3);
    return (seq_t)8 * tmp - (seq_t)3;
}

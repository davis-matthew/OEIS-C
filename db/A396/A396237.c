/*
 * A396237  a(n) = n AND NOT n^2.
 * https://oeis.org/A396237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = n AND NOT n^2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n;
    seq_t sq  = num * num;
    return num & (~sq);
}

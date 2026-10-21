/*
 * A252814  Number of n X 2 nonnegative integer arrays with upper left 0 and
 *          every value within 2 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n⁴ + 10·n³ – n² + 14·n + 24) / 24
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    seq_t n4 = n3 * n0;

    seq_t num = n4 + 10 * n3 - n2 + 14 * n0 + 24;
    return num / 24;
}

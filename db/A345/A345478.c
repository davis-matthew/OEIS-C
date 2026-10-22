/*
 * A345478  Numbers that are the sum of seven squares in one or more ways.
 * https://oeis.org/A345478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 13 for n > 7; initial values: a(1)=7, a(2)=10,
 *            a(3)=13, a(4)=15, a(5)=16, a(6)=18, a(7)=19
 * Range:     u8 n <= 242, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = { (seq_t)7, (seq_t)10, (seq_t)13,
                                   (seq_t)15, (seq_t)16, (seq_t)18,
                                   (seq_t)19 };
    if (n <= 7)
        return init[n - 1];
    return (seq_t)n + 13;
}

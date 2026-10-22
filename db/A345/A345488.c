/*
 * A345488  Numbers that are the sum of eight squares in one or more ways.
 * https://oeis.org/A345488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 14 for n > 7 a(1)=8, a(2)=11, a(3)=14, a(4)=16,
 *            a(5)=17, a(6)=19, a(7)=20
 * Range:     u8 n <= 241, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7) {
        static const seq_t init[7] = {8, 11, 14, 16, 17, 19, 20};
        return init[n - 1];
    }
    return (seq_t)n + 14;
}

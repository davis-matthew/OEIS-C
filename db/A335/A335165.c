/*
 * A335165  Nim n-th power of 6.
 * https://oeis.org/A335165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+15) = a(n); * a(0)=1, a(1)=6, a(2)=5, a(3)=8, a(4)=7,
 *            a(5)=3, a(6)=13, a(7)=15, * a(8)=4, a(9)=14, a(10)=2, a(11)=11,
 *            a(12)=10, a(13)=12, a(14)=9
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[15] = {
        (seq_t)1,  (seq_t)6,  (seq_t)5,  (seq_t)8,  (seq_t)7,
        (seq_t)3,  (seq_t)13, (seq_t)15, (seq_t)4,  (seq_t)14,
        (seq_t)2,  (seq_t)11, (seq_t)10, (seq_t)12, (seq_t)9
    };
    return period[n % 15];
}

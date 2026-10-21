/*
 * A267737  Number of nX3 arrays containing 3 copies of 0..n-1 with every
 *          element equal to or 1 greater than any west or northeast neighbors
 *          modulo n and the upper left element equal to 0.
 * https://oeis.org/A267737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=10, a(3)=17; * for n >= 4 a(n) = 2*a(n-1) - (n is
 *            odd ? 7 : 8) (i.e. subtract 7 on odd n, 8 on even n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)17;

    seq_t cur = (seq_t)17;  /* value for n = 3 */
    for (idx_t i = 4; i <= n; ++i) {
        cur = cur * (seq_t)2;
        cur = cur - ((i & 1) ? (seq_t)7 : (seq_t)8);
    }
    return cur;
}

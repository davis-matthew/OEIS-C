/*
 * A245338  Sum of digits of n written in fractional base 9/8.
 * https://oeis.org/A245338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024656(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t v = n;
    while (v > 0) {
        sum += (seq_t)(v % 9);
        v = (v / 9) * 8;
    }
    return sum;
}

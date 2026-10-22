/*
 * A319006  Sum of the next n positive integers repeated (A008619).
 * https://oeis.org/A319006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n^2 + 2) + (1 - (-1)^n)*(-1)^((n-1)/2))/8
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t first = (seq_t)2 * sn * (sn * sn + (seq_t)2);

    if ((n & 1) == 0) {
        return first / (seq_t)8;
    } else {
        idx_t half = (n - 1) >> 1;
        seq_t numerator;
        if ((half & 1) == 0) {
            numerator = first + (seq_t)2;
        } else {
            numerator = first - (seq_t)2;
        }
        return numerator / (seq_t)8;
    }
}

/*
 * A276191  Sum of the squares of the digits of the base-5 representation of
 *          n.
 * https://oeis.org/A276191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the squares of the digits of the base‑5
 *            representation of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t sq[5] = {0, 1, 4, 9, 16};
    seq_t sum = 0;
    idx_t m = n;
    while (m > 0) {
        unsigned d = (unsigned)(m % 5);
        sum += sq[d];
        m /= 5;
    }
    return sum;
}

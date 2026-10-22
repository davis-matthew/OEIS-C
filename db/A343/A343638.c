/*
 * A343638  a(n) = (Sum of decimal digits of 3*n) / 3.
 * https://oeis.org/A343638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of decimal digits of 3*n) / 3
 * Range:     u8 n <= 85, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)3;   /* multiply n by 3 */
    seq_t sum = 0;

    /* sum decimal digits of m */
    while (m) {
        sum += m % (seq_t)10;
        m /= (seq_t)10;
    }

    return sum / (seq_t)3;
}

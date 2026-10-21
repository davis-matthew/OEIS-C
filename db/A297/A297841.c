/*
 * A297841  a(n) = Sum_{d|n} max(d, n/d)^2.
 * https://oeis.org/A297841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} max(d, n/d)^2
 * Range:     u8 n <= 11, u16 n <= 155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            if (d == other) {
                sum += (seq_t)d * (seq_t)d;
            } else {
                sum += (seq_t)other * (seq_t)other * 2;
            }
        }
    }
    return sum;
}

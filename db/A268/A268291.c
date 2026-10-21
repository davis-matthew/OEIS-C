/*
 * A268291  a(n) = Sum_{k = 0..n} (k mod 13).
 * https://oeis.org/A268291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n + r*(r-11)/2, where r = n mod 13
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = n % 13;
    seq_t six_n = (seq_t)6 * (seq_t)n;
    if (r < 11) {
        seq_t sub = (r * (11 - r)) / 2;
        return six_n - sub;
    } else {
        seq_t add = (r * (r - 11)) / 2;
        return six_n + add;
    }
}

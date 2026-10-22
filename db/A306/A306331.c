/*
 * A306331  Numbers congruent to 6 or 31 mod 38.
 * https://oeis.org/A306331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*n - 13 when n is odd, 19*n - 7 when n is even
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)19 * n;
    if (n & 1ULL) {
        return base - (seq_t)13;
    } else {
        return base - (seq_t)7;
    }
}

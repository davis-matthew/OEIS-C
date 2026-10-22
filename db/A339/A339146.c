/*
 * A339146  a(n) = a(floor(n / 5)) * (n mod 5 + 1); initial terms are 1.
 * https://oeis.org/A339146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(n/5)) * (n mod 5 + 1); a(0) = 1
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    while (n >= 5) {
        res *= (seq_t)(n % 5 + 1);
        n /= 5;
    }
    return res;
}

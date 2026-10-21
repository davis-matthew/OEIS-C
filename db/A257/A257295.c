/*
 * A257295  Arithmetic mean of the digits of n, rounded to the nearest
 *          integer.
 * https://oeis.org/A257295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = round( digit_sum(n)/num_digits(n) )
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t t = n;
    seq_t sum = 0;
    idx_t cnt = 0;
    while (t > 0) {
        sum += (seq_t)(t % 10);
        t /= 10;
        cnt++;
    }
    return (sum + (seq_t)(cnt / 2)) / (seq_t)cnt;
}

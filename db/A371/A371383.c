/*
 * A371383  a(n) is the numerator of the arithmetic mean of the digits of n.
 * https://oeis.org/A371383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, frac, look
 * Formula:   a(n) = digit_sum(n) / gcd(digit_sum(n), num_digits(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t digit_sum_and_count(idx_t n, idx_t *cnt)
{
    seq_t sum = 0;
    if (n == 0) {
        *cnt = 1;
        return sum;
    }
    *cnt = 0;
    while (n > 0) {
        sum += n % 10;
        n   /= 10;
        ++(*cnt);
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t  cnt;
    seq_t  sum = digit_sum_and_count(n, &cnt);
    seq_t  digit_count = (seq_t)cnt;
    seq_t  g = gcd(sum, digit_count);
    return sum / g;
}

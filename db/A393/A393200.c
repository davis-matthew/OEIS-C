/*
 * A393200  a(n) is the least value of m such that the sum of the digits of n
 *          in base m is coprime with n.
 * https://oeis.org/A393200
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m ≥ 2 such that gcd(s_m(n), n) = 1, where s_m(n) is
 *            the sum of the digits of n in base m
 * Range:     u8 n <= 349, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t digit_sum_in_base(seq_t n, seq_t base)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += n % base;
        n /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 2;          /* offset ambiguity; never used for n≥2 */

    seq_t n_seq = (seq_t)n;
    for (idx_t m = 2; m <= n; ++m) {
        seq_t sum = digit_sum_in_base(n_seq, (seq_t)m);
        if (gcd(sum, n_seq) == 1)
            return (seq_t)m;
    }
    return (seq_t)n;    /* should never reach here, base n always works */
}

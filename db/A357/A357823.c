/*
 * A357823  a(n) is the number of bases > 1 where n is not divisible by the
 *          sum of its digits.
 * https://oeis.org/A357823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases b, 2 <= b <= n-1, where n is NOT
 *            divisible by the sum of its digits when n is written in base b
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(n) = (n-2) - #{b: 2 <= b <= n-1, s_b(n) | n}  */

#include <stdint.h>

static inline seq_t digit_sum_base(idx_t value, idx_t base)
{
    seq_t sum = 0;
    idx_t tmp = value;
    while (tmp) {
        sum += (seq_t)(tmp % base);
        tmp /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t nondiv = 0;          /* count of bases where n is NOT divisible by digit sum */
    for (idx_t b = 2; b < n; ++b) {
        seq_t s = digit_sum_base(n, b);
        if (((seq_t)n % s) != 0) {
            ++nondiv;
        }
    }
    return nondiv;
}

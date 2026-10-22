/*
 * A363925  Expansion of Sum_{k>0} x^k / (1 - x^(5*k))^2.
 * https://oeis.org/A363925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d|n, d==1 mod 5} (d+4)
 * Range:     u8 n <= 1055, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;

    /* enumerate divisors i and n/i */
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 % 5 == 1) {
                sum += (seq_t)d1 + 4;
            }

            idx_t d2 = n / i;
            if (d2 != d1 && d2 % 5 == 1) {
                sum += (seq_t)d2 + 4;
            }
        }
    }

    /* each term added to sum is a multiple of 5, so division is exact */
    return sum / 5;
}

/*
 * A396708  Numerators of Sum_{k=1..n} n^2/(k^2 + (n-k)^2).
 * https://oeis.org/A396708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of Σ_{k=1..n} n^2/(k^2 + (n−k)^2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"


seq_t a(idx_t n)
{
    /* Sum as a reduced fraction sum_num / sum_den */
    seq_t sum_num = 0;
    seq_t sum_den = 1;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* Add pair contributions for k = 1 .. floor((n−1)/2) */
    idx_t half = (n - 1) / 2;
    for (idx_t k = 1; k <= half; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t nk   = (seq_t)(n - k);
        seq_t denom = kk * kk + nk * nk;
        seq_t term_num = n_sq * 2;

        seq_t new_num = sum_num * denom + term_num * sum_den;
        seq_t new_den = sum_den * denom;
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    /* Add middle term for even n: value = 2 */
    if ((n & 1) == 0) {
        sum_num = sum_num + (seq_t)2 * sum_den;
        seq_t g = gcd(sum_num, sum_den);
        sum_num /= g;
        sum_den /= g;
    }

    /* Add remaining term k = n: value = 1 */
    sum_num = sum_num + sum_den;
    seq_t g = gcd(sum_num, sum_den);
    sum_num /= g;
    /* sum_den becomes 1 after reduction */

    return sum_num;
}

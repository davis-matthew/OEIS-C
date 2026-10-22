/*
 * A318492  a(n) is the denominator of Sum_{d|n} Sum_{j|d} 1/j.
 * https://oeis.org/A318492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} sigma(d)/d, computed by using a
 *            common denominator n. For each divisor d of n, add sigma(d) *
 *            (n/d) to a numerator, then reduce by gcd(numerator, n)
 * Range:     u8 n <= 23, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t sum_num = 0;
    /* iterate over all divisors of n */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t term1 = sigma_of(d1) * ((seq_t)(n / d1));
            sum_num += term1;
            /* divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = sigma_of(d2) * ((seq_t)(n / d2));
                sum_num += term2;
            }
        }
    }
    seq_t g = gcd(sum_num, (seq_t)n);
    return (seq_t)(n / g);
}

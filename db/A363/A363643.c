/*
 * A363643  Expansion of Sum_{k>0} x^(2*k)/(1 - k*x^k)^3.
 * https://oeis.org/A363643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(d-2) * binomial(d,2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 > 1) {
                seq_t base = (seq_t)(n / d1);
                seq_t exp  = (seq_t)(d1 - 2);
                seq_t pow  = big_pow(base, exp);
                seq_t binom = (seq_t)d1 * (d1 - 1) / 2;
                total += pow * binom;
            }
            if (d2 != d1 && d2 > 1) {
                seq_t base = (seq_t)(n / d2);
                seq_t exp  = (seq_t)(d2 - 2);
                seq_t pow  = big_pow(base, exp);
                seq_t binom = (seq_t)d2 * (d2 - 1) / 2;
                total += pow * binom;
            }
        }
    }
    return total;
}

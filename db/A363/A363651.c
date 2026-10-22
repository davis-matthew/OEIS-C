/*
 * A363651  Expansion of Sum_{k>0} x^(2*k)/(1 - (k*x)^k)^3.
 * https://oeis.org/A363651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-2*n/d) * binomial(d,2)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 >= 2) {
                idx_t nd1 = n / d1;
                seq_t base1 = (seq_t)nd1;
                idx_t exp1 = n - 2 * nd1;
                seq_t term1 = big_pow(base1, (seq_t)exp1) * binomial(d1, 2);
                sum += term1;
            }

            idx_t d2 = n / i;
            if (d2 != d1 && d2 >= 2) {
                idx_t nd2 = n / d2;
                seq_t base2 = (seq_t)nd2;
                idx_t exp2 = n - 2 * nd2;
                seq_t term2 = big_pow(base2, (seq_t)exp2) * binomial(d2, 2);
                sum += term2;
            }
        }
    }
    return sum;
}

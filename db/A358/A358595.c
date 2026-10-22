/*
 * A358595  a(n) = n! * Sum_{d|n} d^n / d!^(n/d).
 * https://oeis.org/A358595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} d^n / d!^(n/d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t nfac = factorial(n);

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* Process divisor d = i */
            idx_t d = i;
            seq_t dfac = factorial(d);
            idx_t exp = n / d;
            seq_t denom = big_pow(dfac, (seq_t)exp);
            seq_t term = nfac / denom;
            seq_t dpow = big_pow((seq_t)d, (seq_t)n);
            term = term * dpow;
            sum += term;

            /* Process complementary divisor d = n/i, if distinct */
            if (i != n / i) {
                d = n / i;
                dfac = factorial(d);
                exp = n / d;  /* equals i */
                denom = big_pow(dfac, (seq_t)exp);
                term = nfac / denom;
                dpow = big_pow((seq_t)d, (seq_t)n);
                term = term * dpow;
                sum += term;
            }
        }
    }
    return sum;
}

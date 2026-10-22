/*
 * A363650  Expansion of Sum_{k>0} x^k/(1 - (k*x)^k)^3.
 * https://oeis.org/A363650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^(n-n/d) * binomial(d+1,2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 29,
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
    /* n should be >= 1. If 0, return 0. */
    if (n == 0)
        return 0;

    seq_t result = 0;
    /* Enumerate divisors of n. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* First divisor i */
            idx_t d = i;
            idx_t quotient = n / d;              /* n/d */
            idx_t exp = n - quotient;           /* n - n/d */
            seq_t pow_val = big_pow((seq_t)quotient, (seq_t)exp);
            seq_t bin_val = binomial(d + 1, 2);  /* (d+1 choose 2) */
            result += pow_val * bin_val;

            /* Second divisor n/i if distinct */
            d = n / i;
            if (d != i) {
                quotient = n / d;              /* which is i */
                exp = n - quotient;           /* n - n/d */
                pow_val = big_pow((seq_t)quotient, (seq_t)exp);
                bin_val = binomial(d + 1, 2);
                result += pow_val * bin_val;
            }
        }
    }
    return result;
}

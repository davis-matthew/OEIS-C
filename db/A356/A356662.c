/*
 * A356662  a(n) = n! * Sum_{d|n} 1/(d!)^(n/d - 1).
 * https://oeis.org/A356662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 1 / (d!)^(n/d - 1) Offset: 1 (a(1) is the
 *            first term)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* find all divisors of n (n is at least 1) */
    uint64_t divisors[10001];
    size_t divcnt = 0;
    uint64_t un = (uint64_t)n;
    for (uint64_t d = 1; d * d <= un; ++d) {
        if (un % d == 0) {
            divisors[divcnt++] = d;
            if (d != un / d) {
                divisors[divcnt++] = un / d;
            }
        }
    }

    /* n! once */
    seq_t n_fac = factorial(n);
    seq_t sum = 0;

    /* iterate over each divisor */
    for (size_t i = 0; i < divcnt; ++i) {
        uint64_t d = divisors[i];
        /* (d!)^(n/d - 1) */
        seq_t d_fac = factorial((idx_t)d);
        idx_t exp = n / d - 1;                     /* integer exponent */
        seq_t denom = big_pow(d_fac, (seq_t)exp);   /* pow(d_fac, exp) */
        seq_t term = n_fac / denom;                 /* divide n! by denom */
        sum += term;
    }
    return sum;
}

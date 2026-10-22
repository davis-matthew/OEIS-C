/*
 * A369338  Sum of all the numbers less than each divisor d of n that do not
 *          divide d.
 * https://oeis.org/A369338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} ( d*(d-1)/2 - (sigma(d)-d) )
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where sigma(d) = sum of all divisors of d */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Sum of divisors of m (sigma(m)) */
static seq_t sigma_of(idx_t m)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t term = 1;
        seq_t power = 1;
        for (int j = 0; j < (int)exps[i]; ++j) {
            power *= p;
            term += power;
        }
        result *= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum_d = 0, sum_d2 = 0, sum_sig = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t s1 = (seq_t)d1;
            sum_d += s1;
            sum_d2 += s1 * s1;
            sum_sig += sigma_of(d1);

            if (d1 != d2) {
                seq_t s2 = (seq_t)d2;
                sum_d += s2;
                sum_d2 += s2 * s2;
                sum_sig += sigma_of(d2);
            }
        }
    }
    seq_t term1 = (sum_d2 - sum_d) / 2;
    return term1 - sum_sig + sum_d;
}

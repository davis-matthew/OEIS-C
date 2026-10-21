/*
 * A285896  Sum of divisors d of n such that n/d is not congruent to 0 mod 5.
 * https://oeis.org/A285896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 5^v5(n) * σ(n / 5^v5(n)) (OEIS A285896)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"              /* provides factor_u64 */

static seq_t sigma_of_uint64(uint64_t m)
{
    if (m <= 1) return (seq_t)1;            /* σ(1) = 1, σ(0) not used */

    /* maximal number of distinct prime factors for 64-bit is 12,
       but 64 slots are safe for any exponent combination. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t sigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        idx_t e = (idx_t)ex[i];

        seq_t term = (seq_t)1;   /* p^0 */
        seq_t sum  = (seq_t)1;   /* 1 + p + p^2 + ... + p^e */

        for (idx_t j = 0; j < e; ++j) {
            term = term * p;
            sum  = sum + term;
        }
        sigma = sigma * sum;
    }
    return sigma;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined for n >= 1 */

    /* Find exponent of 5 in n */
    idx_t k = 0;
    uint64_t tmp = (uint64_t)n;
    while (tmp % 5 == 0) {
        ++k;
        tmp /= 5;
    }

    /* 5^k multiplier */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        pow5 = pow5 * (seq_t)5;
    }

    /* σ(n / 5^k) */
    seq_t sigma = sigma_of_uint64(tmp);

    return pow5 * sigma;
}

/*
 * A349211  a(n) = Sum_{d|n} d^((d+1) mod 2).
 * https://oeis.org/A349211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^((d+1) mod 2) (i.e. sum d if d even, else 1)
 * Range:     u8 n <= 33, u16 n <= 6981, u32 n <= 9259, u64 n <= 9259,
 *            u128 n <= 9259, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset does not matter for n=0 */

    /* Separate the power of 2 from n */
    idx_t t = n;
    idx_t e = 0;                          /* exponent of 2 in n */
    while ((t & 1ULL) == 0) {
        ++e;
        t >>= 1;
    }
    uint64_t m = (uint64_t)t;             /* odd part of n */

    /* Factor the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);    /* fills primes[], exps[], and cnt */

    /* Compute number of odd divisors d(m) and sum of odd divisors sigma(m) */
    seq_t d_m = (seq_t)1;                  /* divisor count of m */
    seq_t sigma_m = (seq_t)1;             /* sum of divisors of m */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t a = exps[i];

        d_m = d_m * (seq_t)(a + 1);

        seq_t num = pow_uint(p, (idx_t)(a + 1));
        num = num - 1;
        seq_t term = num / (seq_t)(p - 1); /* (p^(a+1)-1)/(p-1) */
        sigma_m = sigma_m * term;
    }

    /* Sum over even divisors: sigma(m) * (2^(e+1)-2) */
    seq_t even_sum = (seq_t)0;
    if (e > 0) {
        seq_t pow2 = pow_uint(2, e + 1);
        seq_t even_mult_no = pow2 - 2;         /* 2^(e+1)-2 */
        even_sum = sigma_m * even_mult_no;
    }

    /* Total sum = odd divisor contribution (just count) + even divisor contribution */
    seq_t result = even_sum + d_m;
    return result;
}

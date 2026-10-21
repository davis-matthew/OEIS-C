/*
 * A288418  a(n) = Sum_{d|n} d^2*A000593(n/d).
 * https://oeis.org/A288418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^2*A000593(n/d) Multiplicative form: a(1) = 1
 *            a(n) = ∏ g(p,e) for n = ∏ p^e g(2,e) = (4^(e+1)-1)/3 g(p>2,e) =
 *            ((p^(e+1)-1)*(p^(e+2)-1))/((p-1)*(p^2-1))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 812, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1; no effect on tests */

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = 0;

        if (p == 2) {
            /* g(2,e) = (4^(e+1)-1)/3 */
            seq_t pow4 = (seq_t)1;
            for (uint64_t k = 0; k < e + 1; ++k)
                pow4 *= (seq_t)4;
            term = (pow4 - (seq_t)1) / (seq_t)3;
        } else {
            /* g(p>2,e) = ((p^(e+1)-1)*(p^(e+2)-1))/((p-1)*(p^2-1)) */
            seq_t p_seq = (seq_t)p;
            seq_t p_pow_e1 = (seq_t)1;
            for (uint64_t k = 0; k < e + 1; ++k)
                p_pow_e1 *= p_seq;          /* p^(e+1) */
            seq_t p_pow_e2 = p_pow_e1 * p_seq;   /* p^(e+2) */

            seq_t num1 = p_pow_e1 - (seq_t)1;
            seq_t num2 = p_pow_e2 - (seq_t)1;
            seq_t numerator = num1 * num2;

            seq_t p2 = p_seq * p_seq;          /* p^2 */
            seq_t denom = (seq_t)(p - 1) * (p2 - (seq_t)1);

            term = numerator / denom;
        }

        result *= term;
    }

    return result;
}

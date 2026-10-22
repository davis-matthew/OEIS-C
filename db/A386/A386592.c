/*
 * A386592  Sum of the divisors of n that are not balanced numbers.
 * https://oeis.org/A386592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} d * (1 - [phi(d) divides sigma(d)])
 * Range:     u8 n <= 22, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors of n and add those whose phi(d)
   does NOT divide sigma(d).  The parameters are:
     idx  – current index in the prime factor list
     cnt  – total number of distinct prime factors
     pr   – array of prime bases
     ex   – corresponding exponents
     curr – product of chosen prime powers so far (the divisor)
     phi  – phi(curr) computed incrementally
     sigma– sigma(curr) computed incrementally
     sum  – accumulator for the result
*/
static void rec_divs(size_t idx, size_t cnt,
                     uint64_t *pr, uint64_t *ex,
                     seq_t curr, seq_t phi, seq_t sigma,
                     seq_t *sum)
{
    if (idx == cnt) {
        if (sigma % phi != 0) {
            *sum += curr;
        }
        return;
    }

    uint64_t p = pr[idx];
    seq_t pow_f = 1;                /* p^f */
    for (uint64_t f = 0; f <= ex[idx]; ++f) {
        seq_t phi_new = phi;
        seq_t sigma_new = sigma;

        if (f > 0) {
            seq_t pow_prev = pow_f / (seq_t)p;          /* p^(f‑1) */
            seq_t phi_factor = pow_prev * (seq_t)(p - 1);
            phi_new = phi * phi_factor;

            seq_t pow_next = pow_f * (seq_t)p;          /* p^(f+1) */
            seq_t sigma_factor = (pow_next - 1) / (seq_t)(p - 1);
            sigma_new = sigma * sigma_factor;
        }

        rec_divs(idx + 1, cnt, pr, ex, curr * pow_f,
                 phi_new, sigma_new, sum);
        pow_f *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* n >= 1 by definition, safe guard */

    uint64_t pr[16];
    uint64_t ex[16];
    size_t   cnt = 0;

    /* Factor n.  factor_u64 returns the distinct primes and their exponents. */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sum = 0;
    rec_divs(0, cnt, pr, ex, 1, 1, 1, &sum);
    return sum;
}

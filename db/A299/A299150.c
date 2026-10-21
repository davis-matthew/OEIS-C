/*
 * A299150  Denominators of the positive solution to n = Sum_{d|n} a(d) *
 *          a(n/d).
 * https://oeis.org/A299150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = 2^exp(n) where exp(n) is computed from n’s prime
 *            factorization via the exact recurrence a(n) = 2^exp(n). For each
 *            prime power p^e, exp(p^e) is found by the recurrence a_e = (p^e
 *            - sum_{k=1}^{e-1} a_k a_{e-k})/2. The function below implements
 *            this recurrence for all e≥1 and returns the expo...
 * Range:     u8 n <= 242, u16 n <= 7599, u32 n <= 7599, u64 n <= 7599,
 *            u128 n <= 7599, big any n
 * Helpers:   denom_exp_prime_power, factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/factor_u64.h"
#include "../utils/denom_exp_prime_power.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    /* factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* sum the denominator exponents for each prime power */
    idx_t total_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        total_exp += denom_exp_prime_power(primes[i], exps[i]);
    }
    return pow2(total_exp);
}

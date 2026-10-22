/*
 * A389451  Sum of divisors of the largest unitary divisor of n that is a term
 *          in A048103 (not divisible by p^p for any prime p).
 * https://oeis.org/A389451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p + … + p^e) if e < p, otherwise 1
 *            (multiplicative sequence). The function uses factor_u64 to
 *            obtain the prime factorisation of n
 * Range:     u8 n <= 125, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the defined offset; for safety return 1 for n==0 */
    if (n == 0) return (seq_t)1;

    /* Prime factors and exponents – 64-bit buffer is sufficient for uint64_t n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* Skip the prime if e ≥ p (contribution = 1) */
        if (e >= p) continue;

        seq_t pseq   = (seq_t)p;
        seq_t sum    = (seq_t)1;
        seq_t power  = (seq_t)1;

        for (uint64_t k = 0; k < e; ++k) {
            power *= pseq;   /* power = p^k+1 */
            sum   += power; /* accumulate 1 + p + … + p^e */
        }

        result *= sum;
    }

    return result;
}

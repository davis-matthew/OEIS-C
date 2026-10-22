/*
 * A351267  Sum of the 4th powers of the squarefree divisors of n.
 * https://oeis.org/A351267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^4 * mu(d)^2 Equivalent to: a(1)=1 and for
 *            n>1, a(n)=∏_{p|n} (1+p^4) (multiplicativity)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"    /* provides factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* Return 1 for n == 1 (empty product) */
    if (n == 1) return (seq_t)1;

    /* Workspace for factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n; primes[i] holds the i-th distinct prime divisor,
       exps[i] its exponent (not needed for this calculation). */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    /* Multiply (1 + p^4) for each distinct prime divisor p */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2 = p * p;     /* p^2 */
        seq_t p4 = p2 * p2;   /* p^4 */
        result *= (seq_t)(1) + p4;
    }

    return result;
}

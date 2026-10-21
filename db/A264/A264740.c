/*
 * A264740  Sum of odd parts of divisors of n.
 * https://oeis.org/A264740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, mult
 * Formula:   Multiplicative: a(2^k) = k + 1, a(p^k) = σ(p^k) =
 *            (p^(k+1)-1)/(p-1) for odd p
 * Range:     u8 n <= 16, u16 n <= 1464, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes the sum of odd parts of all divisors of n (OEIS A264740). */
seq_t a(idx_t n)
{
    /* Factorise n. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            /* a(2^k) = k + 1 */
            result *= (seq_t)(e + 1);
        } else {
            /* Compute σ(p^e) = (p^(e+1)-1)/(p-1) */
            seq_t power = 1;          /* p^(e+1) */
            for (uint64_t j = 0; j < e + 1; ++j)
                power *= (seq_t)p;

            seq_t sigma = (power - 1) / (seq_t)(p - 1);
            result *= sigma;
        }
    }

    return result;
}

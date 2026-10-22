/*
 * A389171  a(n) = n + A048146(n) - A057723(n), where A048146 is sum of
 *          non-unitary divisors and A057723 is sum of positive divisors of n
 *          that are divisible by every prime that divides n.
 * https://oeis.org/A389171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + A048146(n) - A057723(n) A048146(n) = sum of
 *            non‑unitary divisors of n A057723(n) = sum of divisors of n that
 *            are divisible by every prime divisor of n
 * Range:     u8 n <= 16, u16 n <= 1573, u32 n <= 9748, u64 n <= 9748,
 *            u128 n <= 9748, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factorize n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* pre‑computed products */
    seq_t sigma   = (seq_t)1;  /* sigma(n) */
    seq_t U       = (seq_t)1;  /* sum of unitary divisors */
    seq_t V       = (seq_t)1;  /* sum of divisors divisible by all primes */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p^e as seq_t */
        seq_t p_pow_e = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k) p_pow_e *= (seq_t)p;

        /* contribution to sigma(n) */
        seq_t p_pow_e_plus1 = p_pow_e * (seq_t)p;
        seq_t sigma_partial = (p_pow_e_plus1 - (seq_t)1) /
                              ((seq_t)p - (seq_t)1);
        sigma *= sigma_partial;

        /* contribution to sum of unitary divisors */
        U *= ((seq_t)1 + p_pow_e);

        /* contribution to V(n) */
        seq_t V_partial = ((p_pow_e - (seq_t)1) * (seq_t)p) /
                          ((seq_t)p - (seq_t)1);
        V *= V_partial;
    }

    /* a(n) = n + sigma(n) - U - V */
    seq_t result = (seq_t)n + sigma - U - V;
    return result;
}

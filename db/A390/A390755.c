/*
 * A390755  The Euler totient of the smallest powerful number divisible by n.
 * https://oeis.org/A390755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = φ(∏_{p^e||n} p^{max(e,2)})
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: factor n, for each prime p^e, multiply (p‑1)·p^(max(e,2)-1). */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Compute the nth term of sequence A390755. */
seq_t a(idx_t n)
{
    /* Handle the special case n = 1 (empty factorization). */
    if (n == 1)
        return 1;

    /* Factorize the integer n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t beta = (e > 1) ? e : 2;          /* required exponent in the powerful number */

        /* Compute p^(beta-1). */
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < beta - 1; ++j)
            p_pow *= (seq_t)p;

        /* Multiply the contribution of this prime. */
        seq_t term = (seq_t)(p - 1) * p_pow;
        result *= term;
    }

    return result;
}

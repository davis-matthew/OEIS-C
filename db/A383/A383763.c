/*
 * A383763  The sum of unitary divisors of n that are exponentially squarefree
 *          numbers.
 * https://oeis.org/A383763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (1 + p^e) if e is square‑free, otherwise 1.
 *            This is the multiplicative definition of OEIS sequence A383763
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return true iff e is square‑free (no square divisor > 1). */
static bool is_squarefree_uint64(uint64_t e)
{
    if (e < 2) return true;
    for (uint64_t k = 2; k * k <= e; ++k)
        if (e % (k * k) == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    /* Storage for factorisation of n (max 64 prime factors). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e   = exps[i];
        uint64_t p   = primes[i];

        /* Compute p^e as seq_t. */
        seq_t pwr = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j)
            pwr *= (seq_t)p;

        if (is_squarefree_uint64(e))
            result *= (pwr + (seq_t)1);
    }

    return result;
}

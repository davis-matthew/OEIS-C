/*
 * A371084  a(n) = 1 if n is a nonsquarefree number not divisible by p^p for
 *          any prime p, otherwise 0.
 * https://oeis.org/A371084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a non‑squarefree integer not divisible by p^p
 *            for any prime p, otherwise 0. (OEIS A371084)
 * Range:     u8 n <= 9203, u16 n <= 9203, u32 n <= 9203, u64 n <= 9203,
 *            u128 n <= 9203, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 1. Invalid or trivial indices */
    if (n < 2)
        return (seq_t)0;

    /* 2. Allocate buffers for prime factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* 3. Factor n using the provided helper */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* 4. Analyse the factorization */
    bool has_square = false;

    for (size_t i = 0; i < cnt; ++i)
    {
        /* If any exponent >= 2, n is not squarefree */
        if (exps[i] >= 2)
            has_square = true;

        /* If exponent >= prime, then p^p | n – disqualify */
        if (exps[i] >= primes[i])
            return (seq_t)0;
    }

    /* 5. Return 1 if non‑squarefree and passes the p^p test, else 0 */
    return (seq_t)(has_square ? 1 : 0);
}

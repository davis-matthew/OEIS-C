/*
 * A386468  The maximum exponent in the prime factorization of the largest
 *          exponentially squarefree divisor of n.
 * https://oeis.org/A386468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{ largest squarefree integer ≤ v_p(n) | p prime
 *            dividing n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_squarefree_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (the largest exponent in the prime factorization of the largest */
/*          exponentially squarefree divisor of n) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree_small.h"

/* Public sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                           /* a(1) = 0 */

    uint64_t N = (uint64_t)n;

    /* Arrays to receive prime factors and exponents of N.  64 slots are enough,
       since 2^64 has at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factorize N.  This helper is provided by the utils package. */
    factor_u64(N, primes, exps, &cnt);

    seq_t maxexp = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t a_exp = exps[i];
        uint64_t b = a_exp;

        /* Find the largest square‑free integer ≤ a_exp. */
        while (!is_squarefree_small(b))
            --b;

        if (b > maxexp)
            maxexp = (seq_t)b;
    }

    return maxexp;
}

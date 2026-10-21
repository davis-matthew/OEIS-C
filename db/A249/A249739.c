/*
 * A249739  The smallest prime whose square divides n, 1 if n is squarefree.
 * https://oeis.org/A249739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime whose square divides n, 1 if n is squarefree. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute the smallest prime p with p^2 | n, or 1 if n is squarefree. */
seq_t a(idx_t n)
{
    /* n < 4 are always squarefree. */
    if (n < 4) return (seq_t)1;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find the smallest prime whose exponent ≥ 2. */
    uint64_t min_p = (uint64_t)-1;  /* initial sentinel */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2 && primes[i] < min_p) {
            min_p = primes[i];
        }
    }

    /* If none found, n is squarefree. */
    if (min_p == (uint64_t)-1) return (seq_t)1;
    return (seq_t)min_p;
}

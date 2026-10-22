/*
 * A335489  Number of strict permutations of the prime indices of n.
 * https://oeis.org/A335489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is not squarefree; otherwise a(n) = A001221(n)!,
 *            i.e. factorial of the number of distinct prime factors of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"    /* factor_u64 */
#include "../utils/factorial.h"    /* factorial */

seq_t a(idx_t n)
{
    /* seq_t may be an arbitrary‑precision type; use cast carefully. */
    if (n <= 1)          /* n == 1 has no prime factors → 1! = 1 */
        return factorial(0);

    /* Factor n.  primes[i] holds the i‑th prime factor,
       exps[i]   holds its exponent.  sz is the number of distinct primes. */
    uint64_t primes[16], exps[16];
    size_t sz;
    factor_u64((uint64_t)n, primes, exps, &sz);

    /* If any exponent exceeds 1, n is not squarefree.  Return 0. */
    for (size_t i = 0; i < sz; ++i)
        if (exps[i] > 1)
            return (seq_t)0;

    /* Squarefree: return factorial of the number of distinct primes. */
    return factorial((idx_t)sz);
}

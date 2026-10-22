/*
 * A369048  a(n) = 1 if the arithmetic derivative of n is greater than n,
 *          otherwise 0.
 * https://oeis.org/A369048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [d(n) > n] where d is the arithmetic derivative
 * Range:     u8 n <= 95, u16 n <= 9368, u32 n <= 9368, u64 n <= 9368,
 *            u128 n <= 9368, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return 1 if the arithmetic derivative of n exceeds n, otherwise 0. */
seq_t a(idx_t n)
{
    /* Arithmetic derivative of 0 or 1 is 0, so the answer is 0. */
    if (n <= 1)
        return (seq_t)0;

    /* Prime factorisation of n.  factor_u64 writes the factors and
       exponents into the provided arrays and the count into cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Work in seq_t to avoid overflow on large n. */
    seq_t nseq = (seq_t)n;
    seq_t der   = (seq_t)0;

    /* d(n) = Σ (e_i * n / p_i)  for each prime p_i^e_i dividing n. */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (nseq / (seq_t)primes[i]) * (seq_t)exps[i];
        der += term;
    }

    return der > nseq ? (seq_t)1 : (seq_t)0;
}

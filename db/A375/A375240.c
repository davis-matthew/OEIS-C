/*
 * A375240  For squarefree j, a(j*k^2) = j * a(k), a(1) = 1.
 * https://oeis.org/A375240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A375240.
 *  a(1) = 1
 *  For squarefree j, a(j·k²) = j · a(k).
 *  Equivalent: multiplicative, a(pᵉ) = p^{popcount(e)} where popcount(e)
 *  is the number of 1‑bits in the binary representation of e. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, but avoid UB */
        return 0;

    if (n == 1)               /* base case */
        return (seq_t)1;

    /* Temporary buffers for prime factorisation.
       Any 64‑bit integer has at most 16 distinct prime factors. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        /* popcount of the exponent (number of 1‑bits). */
        unsigned pc = (unsigned)__builtin_popcountll(exps[i]);

        /* pᵖᶜ where pc = popcount(e). */
        seq_t mult = seq_pow((seq_t)primes[i], (idx_t)pc);

        /* Multiply into the running product. */
        result = seq_mul(result, mult);
    }

    return result;
}

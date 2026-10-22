/*
 * A347127  a(n) = A327251(n) / A003557(n).
 * https://oeis.org/A347127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = ∏_{p^e‖n} ((p + 1) * e + p) (multiplicative definition)
 * Range:     u8 n <= 29, u16 n <= 2309, u32 n <= 9574, u64 n <= 9574,
 *            u128 n <= 9574, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; n≥1 is guaranteed. For n = 1 the empty product is 1. */
    if (n == 1)
        return (seq_t)1;

    /* Buffers large enough for any 64‑bit integer factorization. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    /* factor_u64 writes the prime factors and their multiplicities into
       the provided arrays. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t term = (p + 1) * e + p;
        result *= term;
    }
    return result;
}

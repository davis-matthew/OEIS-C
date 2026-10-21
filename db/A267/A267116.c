/*
 * A267116  Bitwise-OR of the exponents of primes in the prime factorization
 *          of n.
 * https://oeis.org/A267116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise-OR of the exponents of primes in the prime
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    /* Neat fixed-size buffers – 64 is enough for any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i)
        result |= (seq_t)exps[i];
    return result;
}

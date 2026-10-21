/*
 * A297404  A binary representation of the positive exponents that appear in
 *          the prime factorization of a number, shown in decimal.
 * https://oeis.org/A297404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = binary representation of the set of exponents appearing
 *            in the prime factorization of n, i.e. a(n) = Σ_{e∈exp(n)}
 *            2^(e-1)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Temporary buffers for prime factors and exponents.
       For n up to 64‑bit there are at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e == 0) continue;
        result |= (seq_t)1 << (uint32_t)(e - 1);
    }
    return result;
}

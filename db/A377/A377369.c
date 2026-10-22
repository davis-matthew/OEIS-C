/*
 * A377369  a(n) = total number of bits in the binary representation of the
 *          prime factorization of n (including exponents > 1).
 * https://oeis.org/A377369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: factor n and sum the bit lengths of each prime factor
   and of each exponent > 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return the number of bits required to represent a 64‑bit unsigned integer. */
static inline seq_t bitlen_u64(uint64_t x)
{
    seq_t l = 0;
    while (x)
    {
        ++l;
        x >>= 1;
    }
    return l;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* 64‑bit numbers have at most 64 distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* bits of the prime factor itself */
        sum += bitlen_u64(primes[i]);

        /* bits of the exponent if it is greater than 1 */
        if (exps[i] > 1)
            sum += bitlen_u64(exps[i]);
    }
    return sum;
}

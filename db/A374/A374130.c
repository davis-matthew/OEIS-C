/*
 * A374130  a(n) = 1 if bitwise-XOR of the exponents of primes in the prime
 *          factorization of n is equal to 1, otherwise 0.
 * https://oeis.org/A374130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the bitwise XOR of the exponents in the prime *
 *            factorisation of n is equal to 1, otherwise 0
 * Range:     u8 n <= 9899, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   // use the helper that splits n into primes

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 1 has no prime factors, XOR of empty set is 0. */
        return 0;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];     /* enough for any 64‑bit integer */
    uint64_t exps[64];
    size_t cnt;

    /* factor_u64 fills primes[i], exps[i] with the i‑th prime factor and its exponent. */
    factor_u64(nn, primes, exps, &cnt);

    uint64_t xr = 0;
    for (size_t i = 0; i < cnt; ++i) {
        xr ^= exps[i];
    }

    return (xr == 1) ? 1 : 0;
}

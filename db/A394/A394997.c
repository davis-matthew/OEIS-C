/*
 * A394997  Wiener index of the divisor prime graph of n.
 * https://oeis.org/A394997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)*(tau(n)-1) - (∏_{p^a||n} (2*a+1) - 1) / 2 where
 *            tau(n) = A000005(n) and the product runs over the exponents a of
 *            the prime factorization of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The maximum number of distinct prime factors of any 64‑bit integer
       is 15 (product of the first 16 primes already exceeds 2^64).  A
       buffer of 64 is therefore ample. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n.  factor_u64 writes the list of prime factors in 'primes'
       and their exponents in 'exps', and sets *cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t tau = (seq_t)1;        /* τ(n) – number of divisors      */
    seq_t prod = (seq_t)1;       /* product of (2*a_k+1)           */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        tau = tau * (seq_t)(e + 1);
        prod = prod * (seq_t)(2 * e + 1);
    }

    seq_t part1 = tau * (tau - (seq_t)1);
    seq_t part2 = (prod - (seq_t)1) / (seq_t)2;
    return part1 - part2;
}

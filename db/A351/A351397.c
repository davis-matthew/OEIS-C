/*
 * A351397  Sum of the exponents in the prime factorizations of the prime
 *          power divisors of n.
 * https://oeis.org/A351397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p^e‖n} e*(e+1)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes A351397 – sum of exponents in prime–power divisors of n */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0; for n <= 1 return 0 directly */
    if (n <= 1)
        return 0;

    uint64_t u = (uint64_t)n;
    /* arrays to receive the factorisation of n */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    /* factor_u64 populates |primes|, |exponents| and |cnt| */
    factor_u64(u, primes, exponents, &cnt);

    seq_t total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exponents[i];
        /* e*(e+1)/2 fits into seq_t whenever it fits into uint64_t */
        seq_t term = ((seq_t)e * ((seq_t)e + 1)) / 2;
        total += term;
    }
    return total;
}

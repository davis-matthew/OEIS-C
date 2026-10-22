/*
 * A359474  a(n) = 1 if the product of exponents in the prime factorization of
 *          n is 2, otherwise 0.
 * https://oeis.org/A359474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the product of exponents in the prime factorization
 *            of n is 2, otherwise 0
 * Range:     u8 n <= 9269, u16 n <= 9269, u32 n <= 9269, u64 n <= 9269,
 *            u128 n <= 9269, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 because the sequence offset is 1; for n==1 the product of exponents is undefined so return 0 */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Convert to native uint64_t for factorisation; n is guaranteed to fit */
    uint64_t nn = (uint64_t)n;

    /* Temporary buffers for prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n */
    factor_u64(nn, primes, exps, &cnt);

    /* Compute product of exponents */
    unsigned prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (unsigned)exps[i];
    }

    /* Return 1 if product == 2, else 0 */
    return (prod == 2) ? (seq_t)1 : (seq_t)0;
}

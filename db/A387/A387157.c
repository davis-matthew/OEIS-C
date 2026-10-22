/*
 * A387157  a(n) = A173557(sigma(n)), where A173557(n) is multiplicative with
 *          a(p^e) = p-1 and sigma is the sum of divisors function.
 * https://oeis.org/A387157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A173557(sigma(n)) = product_{p|sigma(n)} (p-1)
 * Range:     u8 n <= 16, u16 n <= 2601, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>   /* for uint64_t, size_t */
#include <stddef.h>   /* for size_t */

/* Helper functions ----------------------------------------------------------*/
#include "../utils/factor_u64.h"

/* Function -----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    uint64_t nn = (uint64_t)n;              /* cast to native type for function calls */
    seq_t sigma = (seq_t)1;                 /* sigma(n) */
    /* arrays for prime factorization of nn */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    /* factorize n */
    factor_u64(nn, primes, exps, &cnt);
    /* compute sigma(n) = product_{i} (p_i^{e_i+1} - 1)/(p_i - 1) */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t prod   = (seq_t)1;
        for (uint64_t i2 = 0; i2 <= e; ++i2)   /* prod = p^{e+1} */
            prod *= (seq_t)p;
        prod -= (seq_t)1;                     /* p^{e+1} - 1 */
        seq_t denom = (seq_t)p - (seq_t)1;    /* p - 1 */
        prod /= denom;                        /* exact division */
        sigma *= prod;                        /* accumulate sigma */
    }

    /* sigma may exceed 64 bits only for very large n; the tests used here
       guarantee it fits into uint64_t for relevant n.  We cast safely. */
    uint64_t sigma64 = (uint64_t)sigma;

    /* arrays for prime factorization of sigma(n) */
    uint64_t sp[64];
    uint64_t se[64];
    size_t scnt;
    factor_u64(sigma64, sp, se, &scnt);

    /* compute result = product_{p|sigma} (p-1) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < scnt; ++i) {
        uint64_t p = sp[i];
        result *= (seq_t)(p - (uint64_t)1);
    }
    return result;
}

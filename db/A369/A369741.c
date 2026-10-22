/*
 * A369741  a(n) = Sum_{p|n, p prime} p^Omega(n/p).
 * https://oeis.org/A369741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{p|n,\;p\text{ prime}}\,p^{\Omega(n/p)}
 * Range:     u8 n <= 55, u16 n <= 271, u32 n <= 2623, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1)=0 */

    uint64_t nn = (uint64_t)n;     /* convert idx_t to uint64_t for factoring */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(nn, primes, exps, &cnt);

    /* compute Omega(n) = sum of exponents */
    uint64_t omega = 0;
    for (size_t i = 0; i < cnt; ++i) {
        omega += exps[i];
    }

    seq_t exp = (seq_t)(omega - 1);   /* exponent in each term */
    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = big_pow((seq_t)primes[i], exp);
        result += term;
    }
    return result;
}

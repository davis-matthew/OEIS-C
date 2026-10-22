/*
 * A349442  Dirichlet convolution of A000027 (the identity function) with
 *          A349350 (Dirichlet inverse of the powerful part of n).
 * https://oeis.org/A349442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} d * A349350(n/d)
 * Range:     i8 n <= 130, i16 n <= 9793, i32 n <= 9793, i64 n <= 9793,
 *            i128 n <= 9793, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    /* factorisation */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t idx = 0; idx < cnt; ++idx) {
        uint64_t p = primes[idx];
        size_t e = (size_t)exps[idx];

        /* powers of p */
        seq_t pow[64];
        pow[0] = 1;
        for (size_t k = 1; k <= e; ++k) pow[k] = pow[k-1] * (seq_t)p;

        /* Dirichlet inverse of powerful part: g(p^e) */
        seq_t g[64];
        g[0] = 1;
        for (size_t k = 1; k <= e; ++k) {
            seq_t sum = 0;
            for (size_t j = 1; j <= k; ++j) {
                seq_t f = (j == 1) ? 1 : pow[j];
                sum += f * g[k - j];
            }
            g[k] = -sum;
        }

        /* convolution with identity: a(p^e) */
        seq_t a_pe = 0;
        for (size_t k = 0; k <= e; ++k) {
            a_pe += pow[k] * g[e - k];
        }
        result *= a_pe;
    }
    return result;
}

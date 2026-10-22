/*
 * A396945  Number of divisors of n^3 + 1.
 * https://oeis.org/A396945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n^3 + 1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* 0^3 + 1 = 1 has exactly 1 divisor */

    /* two factors of n^3 + 1 */
    uint64_t A = (uint64_t)n + 1ULL;                     /* n + 1      */
    unsigned __int128 t = (unsigned __int128)n * n - n + 1; /* n^2 - n + 1 */
    uint64_t B = (uint64_t)t;                           /* fits for n ≤ 10⁶      */

    /* factor A and B */
    uint64_t prA[20], exA[20];
    size_t cntA = 0;
    factor_u64(A, prA, exA, &cntA);

    uint64_t prB[20], exB[20];
    size_t cntB = 0;
    factor_u64(B, prB, exB, &cntB);

    /* merge exponent vectors; only 3 can appear twice */
    uint64_t primes[40], exps[40];
    size_t total = 0;

    /* copy factors from A */
    for (size_t i = 0; i < cntA; ++i) {
        primes[total] = prA[i];
        exps[total]   = exA[i];
        ++total;
    }

    /* merge factors from B */
    for (size_t j = 0; j < cntB; ++j) {
        uint64_t p = prB[j];
        size_t k;
        for (k = 0; k < total; ++k) {
            if (primes[k] == p) {
                exps[k] += exB[j];
                break;
            }
        }
        if (k == total) {               /* new prime */
            primes[total] = p;
            exps[total]   = exB[j];
            ++total;
        }
    }

    /* compute divisor count from combined exponents */
    seq_t res = 1;
    for (size_t i = 0; i < total; ++i)
        res *= (seq_t)(exps[i] + 1);

    return res;
}

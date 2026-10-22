/*
 * A308593  a(n) = Sum_{d|n} d^(n^2/d).
 * https://oeis.org/A308593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n^2/d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* Undefined for n = 0, but harmless */

    /* Factorize n */
    uint64_t primes[64], exps[64];
    size_t cntPrimes = 0;
    factor_u64((uint64_t)n, primes, exps, &cntPrimes);

    /* Generate all divisors of n */
    size_t divCap = 1;
    size_t divCnt = 1;
    uint64_t *divs = (uint64_t*)malloc(sizeof *divs);
    if (!divs) return 0;
    divs[0] = 1;

    for (size_t pi = 0; pi < cntPrimes; ++pi) {
        uint64_t p = primes[pi];
        uint64_t emax = exps[pi];
        size_t oldCnt = divCnt;

        /* Precompute powers of p */
        uint64_t powers[64];
        powers[0] = 1;
        for (size_t e = 1; e <= emax; ++e) powers[e] = powers[e - 1] * p;

        for (size_t e = 1; e <= emax; ++e) {
            uint64_t mult = powers[e];
            for (size_t j = 0; j < oldCnt; ++j) {
                if (divCnt == divCap) {
                    divCap *= 2;
                    uint64_t *tmp = (uint64_t*)realloc(divs, divCap * sizeof *divs);
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[divCnt++] = divs[j] * mult;
            }
        }
    }

    /* Compute the sum */
    seq_t nBig = (seq_t)n;
    seq_t nSq  = nBig * nBig;
    seq_t sum  = 0;

    for (size_t i = 0; i < divCnt; ++i) {
        uint64_t d = divs[i];
        seq_t exp = nSq / (seq_t)d;
        seq_t term = big_pow((seq_t)d, exp);
        sum += term;
    }

    free(divs);
    return sum;
}

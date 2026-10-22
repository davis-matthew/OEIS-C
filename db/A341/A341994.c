/*
 * A341994  a(n) = 1 if the arithmetic derivative (A003415) of n is a
 *          squarefree number (A005117) > 1, otherwise 0.
 * https://oeis.org/A341994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative of n is a squarefree
 *            integer > 1, otherwise 0
 * Range:     u8 n <= 9798, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }

    /* Compute the arithmetic derivative of n. */
    uint64_t n64 = (uint64_t)n;
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    uint64_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        deriv += exps[i] * (n64 / primes[i]);
    }

    /* Check if der > 1 and squarefree. */
    if (deriv <= 1) {
        return 0;
    }

    /* Factor derivative to test squarefreeness. */
    uint64_t dprimes[32], dexps[32];
    size_t dcnt = 0;
    factor_u64(deriv, dprimes, dexps, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        if (dexps[i] > 1) {
            return 0;
        }
    }
    return 1;
}

/*
 * A339688  a(n) = Sum_{d|n} 8^(d-1).
 * https://oeis.org/A339688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 8^(d-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS sequence A339688. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factorize n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Enumerate all divisors of n.  A stack of 256 is ample for n ≤ 10 000. */
    idx_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        idx_t prime = primes[i];
        idx_t exp   = exps[i];
        size_t old = dcnt;
        idx_t pow = 1;
        for (idx_t e = 1; e <= exp; ++e) {
            pow *= prime;
            for (size_t j = 0; j < old; ++j) {
                divs[dcnt++] = divs[j] * pow;
            }
        }
    }

    /* Find the largest divisor to know how many powers of 8 to pre‑compute. */
    idx_t maxd = 1;
    for (size_t i = 0; i < dcnt; ++i) {
        if (divs[i] > maxd) {
            maxd = divs[i];
        }
    }

    /* Pre‑compute powers of 8: pow8[k] = 8^k for k = 0 … maxd-1. */
    seq_t *pow8 = (seq_t *)malloc(maxd * sizeof(seq_t));
    pow8[0] = (seq_t)1;
    for (idx_t i = 1; i < maxd; ++i) {
        pow8[i] = pow8[i - 1] * (seq_t)8;
    }

    /* Sum 8^(d-1) over all divisors d. */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        sum += pow8[d - 1];
    }

    free(pow8);
    return sum;
}

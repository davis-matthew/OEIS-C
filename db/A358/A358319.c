/*
 * A358319  Multiplicative sequence a(n) with a(p^e) = ((p-2) - (p-1) * e) *
 *          p^(e-1) for prime p and e > 0.
 * https://oeis.org/A358319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = product over prime power factors p^e || n of ((p-2) -
 *            (p-1)*e) * p^(e-1), with a(1)=1 (multiplicative function)
 * Range:     i8 n <= 63, i16 n <= 5183, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;   /* a(1)=1, no terms below offset */

    /* Temporary arrays to hold prime factors and exponents.
       A 64‑bit integer has at most 13 distinct prime factors, so 64 slots is safe. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n using the provided utility. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute p^(e-1) in seq_t. */
        seq_t pow_p = 1;
        for (uint64_t k = 1; k < e; ++k) {
            pow_p *= (seq_t)p;
        }

        /* Compute the coefficient ((p-2) - (p-1)*e). */
        seq_t coeff = (seq_t)(p - 2) - (seq_t)(p - 1) * (seq_t)e;

        /* Multiply into the running product. */
        result *= coeff * pow_p;
    }

    return result;
}

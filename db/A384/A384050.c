/*
 * A384050  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a powerful number.
 * https://oeis.org/A384050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} { p-1 if e=1, p^e if e≥2 } (multiplicative
 *            definition)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute a(n) for n >= 1.  For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* For n == 1 there are no prime factors; the empty product equals 1. */
    if (n == 1) return 1;

    /* Buffers for prime factors and exponents.  The maximum number of distinct
       prime factors of a 64‑bit integer is 16 (2·3·5·7·11·13·17·19·23·29·31·37·41·43·47·53 > 2³¹²⁰⁴⁴⁴¹⁶).
       64 is safely large. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorize n.  factor_u64 must be able to handle 64‑bit n. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 1) {
            /* multiply by (p - 1) */
            result *= (seq_t)(primes[i] - 1);
        } else {
            /* multiply by p^e */
            seq_t pow = 1;
            for (idx_t j = 0; j < (idx_t)exps[i]; ++j) {
                pow *= (seq_t)primes[i];
            }
            result *= pow;
        }
    }
    return result;
}

/*
 * A399828  The number of divisors d of n such that gcud(d, n/d) is
 *          squarefree, where gcud is the greatest common unitary divisor.
 * https://oeis.org/A399828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e||n} (e+1 for e≤2, otherwise e + (e % 2))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Special case n == 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* Convert n to uint64_t for the factorization helper */
    uint64_t nn = (uint64_t)n;

    /* Arrays to hold prime factors and exponents. 2^64 has at most 14 prime factors,
       so 64 slots is more than enough. */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    /* Factor n */
    factor_u64(nn, primes, exponents, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exponents[i];
        seq_t contrib;
        if (e <= 2) {
            contrib = (seq_t)(e + 1);
        } else {
            contrib = (seq_t)(e + (e % 2));
        }
        result *= contrib;
    }
    return result;
}

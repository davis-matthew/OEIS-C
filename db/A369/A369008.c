/*
 * A369008  a(n) = A085731(n) / A003557(n).
 * https://oeis.org/A369008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e || n of p if p | e,
 *            otherwise 1
 * Range:     u8 n <= 9243, u16 n <= 9243, u32 n <= 9243, u64 n <= 9243,
 *            u128 n <= 9243, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include <stddef.h>                     /* size_t */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;               /* a(1) = 1 */

    uint64_t value = (uint64_t)n;       /* factor_u64 expects uint64_t */
    seq_t result = 1;

    /* Allocate space for the largest possible number of distinct prime factors
       of a 64‑bit integer (at most 16, but 64 gives ample headroom). */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t   count;

    factor_u64(value, primes, exponents, &count);

    for (size_t i = 0; i < count; ++i) {
        if (exponents[i] % primes[i] == 0) {
            result *= (seq_t)primes[i];
        }
    }

    return result;
}

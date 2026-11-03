/*
 * A372329  a(n) is the smallest multiple of n whose number of divisors is a
 *          power of 2 (A036537).
 * https://oeis.org/A372329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} p^(2^⌈log₂(e+1)⌉ − 1) (OEIS A372329)
 * Range:     u8 n <= 47, u16 n <= 624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9901 (2026-11-01)
 * Helpers:   factor_u64, ilog2, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper prototypes – provided by the build environment */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ilog2.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 only; n = 0 is never queried. */
    if (n == 0) return 0;

    /* Temporary storage for prime factors and their exponents. */
    uint64_t primes[64], exps[64];
    size_t cnt;

    /* Factorize n using the supplied helper. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        uint64_t ep1 = e + 1;                    /* e + 1 */

        /* k = ceil(log₂(e + 1)) */
        unsigned k = ilog2(ep1);
        if ((1ULL << k) != ep1) {
            ++k;                                 /* make it a ceiling */
        }

        /* exponent for the multiplicative factor: 2^k − 1 */
        uint64_t exp = (1ULL << k) - 1;

        /* Compute p^exp in the target type seq_t. */
        seq_t term = ipow((seq_t)p, (seq_t)exp);

        /* Accumulate the product. */
        result = result * term;
    }

    return result;
}

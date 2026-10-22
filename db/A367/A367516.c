/*
 * A367516  The number of unitary divisors of n that are exponentially evil
 *          numbers (A262675).
 * https://oeis.org/A367516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over p^e||n of (2 - is_binary_weight_odd(e))
 *            where is_binary_weight_odd(e) is 1 if the binary representation
 *            of e contains an odd number of 1's, otherwise 0. Equivalently:
 *            a(n) = 2^{# of prime exponents with even binary weight}
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

static inline int parity_popcount(uint64_t x)
{
    int parity = 0;
    while (x) {
        parity ^= (int)(x & 1ULL);
        x >>= 1;
    }
    return parity;          /* 1 if odd popcount, 0 if even */
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;   /* a(1) = 1 */

    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (parity_popcount(e) == 0) {   /* even binary weight → multiply by 2 */
            result <<= 1;
        }
        /* odd binary weight → multiply by 1 (no change) */
    }
    return result;
}

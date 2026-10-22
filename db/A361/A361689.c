/*
 * A361689  The number of divisors of 2*n^2.
 * https://oeis.org/A361689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of 2*n^2. * The number of divisors of
 *            2*n^2 can be computed from the prime exponents * of n. If n =
 *            2^k * Π p_i^e_i (p_i odd), then * 2*n^2 = 2^(2k+1) * Π
 *            p_i^(2e_i) * so * d(2*n^2) = (2k+2) * Π (2e_i+1)
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but guard against 0 */

    /* Factor n.  Use a small static buffer: 64 factors is ample for uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    bool has2 = false;

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2ULL) {
            has2 = true;
            seq_t k = (seq_t)exps[i];
            result = result * ((seq_t)(2 * k + 2));
        } else {
            seq_t e = (seq_t)exps[i];
            result = result * ((seq_t)(2 * e + 1));
        }
    }

    /* If 2 does not divide n, we still need the factor (2k+2)=2 for k=0. */
    if (!has2) {
        result = result * 2;
    }

    return result;
}

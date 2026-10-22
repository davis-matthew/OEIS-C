/*
 * A366076  The number of prime factors of the largest divisor of n that is a
 *          cubefull number (A036966), counted with multiplicity.
 * https://oeis.org/A366076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of prime exponents e_i in the factorisation of n *
 *            for all e_i >= 3. This is equivalent to A001222(A360540(n)). The
 *            * implementation uses the helper `factor_u64` to factor n and
 *            then * adds up all exponents that are at least 3. It works for
 *            any integer * type seq_t provided by the build
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

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* offset starts at 1 */

    uint64_t x = (uint64_t)n;
    seq_t sum = 0;

    /* Arrays large enough for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    /* Factorisation: primes[i]^{exp[i]} */
    factor_u64(x, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 3) {
            sum += (seq_t)exps[i];
        }
    }
    return sum;
}

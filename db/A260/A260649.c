/*
 * A260649  Expansion of (phi(q^3) * phi(q^5) + phi(q) * phi(q^15)) / 2 - 1 in
 *          powers of q where phi(q) is a Ramanujan theta function.
 * https://oeis.org/A260649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers of a(p^e) according to * the
 *            multiplicative definition in A260649
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_for_prime, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/factor_for_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    /* buffers for prime factors and their exponents – 64 entries
       are sufficient for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];
        seq_t factor = factor_for_prime(p, e);
        result *= factor;
        if (result == (seq_t)0)
            return (seq_t)0;
    }
    return result;
}

/*
 * A366840  Sum of odd prime factors of n, counted with multiplicity.
 * https://oeis.org/A366840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of odd prime factors of n, counted with multiplicity
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function factor_u64 from ../utils/factor_u64.h. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t u = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(u, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] != 2) {           /* skip prime factor 2 */
            sum += (seq_t)primes[i] * (seq_t)exps[i];
        }
    }
    return sum;
}

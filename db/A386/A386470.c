/*
 * A386470  The number of divisors of n whose exponents in their prime
 *          factorization are squares.
 * https://oeis.org/A386470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (⌊√e⌋ + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* integer square‑root (floor) for 64‑bit values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t i = 0;
    while ((i + 1) * (i + 1) <= n) {
        ++i;
    }
    return i;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* undefined for the sequence, but safe guard */
    }

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e   = (idx_t)exps[i];
        idx_t sq  = isqrt_u64(e);
        result *= ((seq_t)sq + 1);
    }
    return result;
}

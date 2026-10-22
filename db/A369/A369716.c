/*
 * A369716  The number of divisors of the smallest powerful number that is a
 *          multiple of n.
 * https://oeis.org/A369716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p|n, e≥2}(e+1) * Π_{p|n, e=1} 3 where n = Π p^e. This
 *            is the divisor count of the smallest powerful multiple of n
 * Range:     u8 n <= 9239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;  /* a(1) = 1 */

    /* A 64‑bit integer can have at most 64 distinct prime factors.
       Allocate static arrays big enough to hold them. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t factor = (exps[i] == 1) ? (seq_t)3 : (seq_t)(exps[i] + 1);
        result *= factor;
    }
    return result;
}

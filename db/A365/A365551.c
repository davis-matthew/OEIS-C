/*
 * A365551  The number of exponentially odd divisors of the smallest
 *          exponentially odd number divisible by n.
 * https://oeis.org/A365551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = ceil((e+3)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* out of range, safe return */

    /* factor_u64 returns prime bases in pr[], exponents in ex[], count in cnt */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1; /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t e = (seq_t)exps[i];                /* exponent of a prime factor */
        seq_t contrib = (e + 4) / 2;             /* ceil((e+3)/2) equals (e+4)/2 */
        result *= contrib;                       /* multiply contributions */
    }

    return result;
}

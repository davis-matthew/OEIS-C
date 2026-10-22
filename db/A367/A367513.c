/*
 * A367513  The exponentially evil part of n: the largest unitary divisor of n
 *          that is an exponentially evil number (A262675).
 * https://oeis.org/A367513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of all prime powers p^e dividing n where * the
 *            number of 1‑bits in the binary expansion of e is even
 * Range:     u8 n <= 342, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* value for indices below the offset is irrelevant */

    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned int pc = __builtin_popcountll(exps[i]); /* popcount of the exponent */
        if ((pc & 1) == 0) {                            /* even number of 1 bits     */
            seq_t term = pow_seq((seq_t)primes[i], (int)exps[i]);
            result = result * term;                      /* multiplication works for
                                                             all seq_t types        */
        }
    }
    return result;
}

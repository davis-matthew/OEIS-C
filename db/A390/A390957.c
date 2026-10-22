/*
 * A390957  The number of non-bi-unitary divisors of n.
 * https://oeis.org/A390957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) - bi_unitary(n) where tau(n) = number of divisors
 *            of n, bi_unitary(n) = number of divisors d of n such that the
 *            greatest common unitary divisor of d and n/d is 1. For a prime
 *            factor p^e of n, bi_unitary choices = e+1 if e is odd, = e if e
 *            is even
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
    /* n = 1 has no prime factors */
    if (n == 1)
        return 0;

    /* factor the number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t tau = 1;        /* product of (e+1) */
    seq_t bi   = 1;       /* product of (e+1) or e */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t exp = (seq_t)exps[i];

        tau *= (exp + 1);

        if ((exps[i] & 1u) == 0)   /* even exponent */
            bi *= exp;             /* exclude the choice f = e/2 */
        else                        /* odd exponent */
            bi *= (exp + 1);
    }

    return tau - bi;    /* non-bi-unitary divisors */
}

/*
 * A345373  Sum of the divisor complements of the unitary prime divisors of n.
 * https://oeis.org/A345373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{p|n, p prime, exponent(p)=1} 1/p Equivalent:
 *            a(n) = Sum_{p|n, exponent(p)=1} n/p (i.e., sum n divided by each
 *            unitary prime divisor)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>         /* for size_t */
#include <stdint.h>        /* for uint64_t */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) == 0 by definition */
        return (seq_t)0;

    uint64_t m = (uint64_t)n; /* n is 64‑bit, fits in uint64_t */

    /* Temporary buffers for the factorization routine.
       A 64‑bit number has at most 16 distinct prime factors. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 1)               /* unitary prime divisor */
            result += (seq_t)(m / primes[i]); /* n/p is an integer */
    }

    return result;
}

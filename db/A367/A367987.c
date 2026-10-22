/*
 * A367987  The number of square divisors of the largest unitary divisor of n
 *          that is a square.
 * https://oeis.org/A367987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} ( e/2 + 1 if e even, 1 if e odd )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, computed multiplicatively using factor_u64. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* In the OEIS offset 1 the first term is for n = 1. */
    /* For n = 0 the behaviour is irrelevant, but handle it safely. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Largest factor array needed for 64‑bit n   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor_u64 writes the distinct primes and their exponents
       into primes[] and exps[], and puts the count into *cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if ((e & 1U) == 0)          /* e even */
            result *= (seq_t)(e / 2U + 1U);
    }
    return result;
}

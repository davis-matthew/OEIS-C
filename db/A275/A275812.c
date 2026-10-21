/*
 * A275812  Sum of exponents larger than one in the prime factorization of n:
 *          A001222(n) - A056169(n).
 * https://oeis.org/A275812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{p^e || n, e>1} e (sum of exponents larger than one)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to A001222(n) - A056169(n). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is of type idx_t (unsigned 64‑bit). Handle n < 2 directly. */
    if (n < 2) return 0;

    /* Temporary arrays for prime factors and exponents. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    /* Factor n. The helper fills primes[], exps[], and cnt. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sum exponents that are > 1. */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] > 1)
            sum += (seq_t)exps[i];
    }
    return sum;
}

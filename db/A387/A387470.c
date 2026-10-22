/*
 * A387470  a(n) = n*(Omega(n) - 1), where Omega = A001222.
 * https://oeis.org/A387470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(Omega(n) - 1) where Omega(n) is the total number of
 *            prime factors of n counted with multiplicity
 * Range:     i8 n <= 31, i16 n <= 200, i32 n <= 200, i64 n <= 200,
 *            i128 n <= 200, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor the argument (uint64_t) into primes and exponents. */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(temp, primes, exps, &cnt);

    /* Compute Omega(n) = sum of exponents. */
    seq_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (seq_t)exps[i];

    /* a(n) = n * (Omega(n) - 1). */
    seq_t result = (seq_t)n * (omega - 1);
    return result;
}

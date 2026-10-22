/*
 * A344326  Dirichlet g.f.: zeta(s)^2/zeta(2*s-1).
 * https://oeis.org/A344326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = multiplicative function with a(p^e) = 2 - (e-1)*(p-1)
 * Range:     i8 n <= 8213, i16 n <= 9771, i32 n <= 9771, i64 n <= 9771,
 *            i128 n <= 9771, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes the n-th term of OEIS sequence A344326.
   n is an unsigned 64‑bit integer, but only values n >= 1 are used. */
seq_t a(idx_t n)
{
    /* a(1) is defined as 1 by the sequence definition. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* n fits into a uint64_t. */
    uint64_t N = (uint64_t)n;

    /* Space for prime factors and their exponents. 128 is ample
       for numbers up to 64 bits. */
    static uint64_t primes[128];
    static uint64_t exps[128];
    size_t cnt = 0;

    /* Factor N; the helper writes the primes into primes[],
       the exponents into exps[], and the count into cnt. */
    factor_u64(N, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        /* Compute a(p^e) = 2 - (e-1)*(p-1).  All calculations use seq_t. */
        seq_t term = (seq_t)2 - ((e - (seq_t)1) * (p - (seq_t)1));

        /* Short‑circuit for zero factor: product will be zero. */
        if (term == (seq_t)0)
            return (seq_t)0;

        result *= term;
    }

    return result;
}

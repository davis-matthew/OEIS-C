/*
 * A347992  a(n) = Sum_{d|n} (-1)^(tau(d) - 1).
 * https://oeis.org/A347992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} (-1)^{τ(d)-1} For any integer n ≥ 1, a(n) = 2 *
 *            (number of square divisors of n) – τ(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Since τ(d) is odd iff d is a perfect square, each divisor
    contributes +1 for squares and –1 otherwise.  The number of
    square divisors of n = Π_i (⌊e_i/2⌋ + 1) for n = Π_i p_i^{e_i}. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute a(n) as defined by the sequence A347992.  */
seq_t a(idx_t n)
{
    /*  n ≥ 1 by definition.  */
    uint64_t num = (uint64_t)n;          /* cast to native integer for factorization */
    size_t   cnt = 0;                    /* number of distinct prime factors */
    uint64_t primes[64];
    uint64_t exps [64];

    /* factor_u64 fills primes, exps, and cnt with the prime factorization of num. */
    factor_u64(num, primes, exps, &cnt);

    seq_t prod_sq  = 1;   /* product of (e_i/2 + 1)  */
    seq_t prod_all = 1;   /* product of (e_i + 1)     */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        prod_sq  *= (seq_t)(e / 2 + 1);
        prod_all *= (seq_t)(e + 1);
    }

    /* for n = 1, cnt = 0 → prod_sq = prod_all = 1:   a(1) = 2*1 - 1 = 1. */
    return 2 * prod_sq - prod_all;
}

/*
 * A351844  a(n) = Sum_{d|n} Sum_{p|n, p prime} n^gcd(d,p).
 * https://oeis.org/A351844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Sum_{p|n, p prime} n^gcd(d,p)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* a(1) = 0: no prime divisors, so inner sum is empty. */
    if (n == (idx_t)1)
        return (seq_t)0;

    /* Factor n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;            /* number of distinct prime factors */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = number of divisors = Π_i (e_i + 1). */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    seq_t n_seq = (seq_t)n;
    seq_t sum   = (seq_t)0;

    /* For each prime factor p_j, add its contribution. */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e      = (seq_t)exps[i];            /* exponent of this prime */
        seq_t tau_div = tau / (e + 1);           /* Π_{k≠i} (e_k+1)  */
        seq_t count   = tau_div * e;             /* number of divisors divisible by p_j */

        seq_t pow_pj = pow_uint((uint64_t)n, primes[i]); /* n^p_j */
        seq_t term   = count * pow_pj + (tau - count) * n_seq;

        sum += term;
    }

    return sum;
}

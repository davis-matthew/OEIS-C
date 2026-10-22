/*
 * A376514  Number of divisors of n that are neither squarefree nor prime
 *          powers.
 * https://oeis.org/A376514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) - 2^omega(n) - bigomega(n) + omega(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, count_distinct_primes, divisor_count_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/bigomega.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* divisor count τ(n) */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* number of distinct prime factors ω(n) */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* total number of prime factors with multiplicity Ω(n) */
    seq_t bigo = bigomega(n);

    /* 2^ω(n) */
    seq_t pow2omega = pow2((idx_t)omega);

    /* compute a(n) = τ(n) - 2^ω(n) - Ω(n) + ω(n) */
    /* to avoid unsigned underflow, compute (τ+ω) - (2^ω+Ω) */
    seq_t res = (tau + omega) - (pow2omega + bigo);

    return res;
}

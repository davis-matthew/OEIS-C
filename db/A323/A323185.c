/*
 * A323185  Denominators of rationals whose continued fraction representations
 *          show the prime factors of n (for n > 1) in nondecreasing order.
 * https://oeis.org/A323185
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A323185: Denominators of rationals whose continued fraction
 * representations display the prime factors of n (for n > 1) in
 * nondecreasing order.
 * 
 * Computation:
 *   1. Factor n into its prime factors (with multiplicity) and sort them.
 *   2. Build the continued fraction [0; p1, p2, ..., pk], where the
 *      p's are the sorted prime factors.
 *   3. Compute the denominator of the rational described by that
 *      continued fraction, using the standard recurrence for convergents.
 * 
 * The result is the denominator of the reduced fraction having that
 * continued fraction.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Handle trivial cases (not part of OEIS range, but safe). */
    if (n < 2) return (seq_t)1;

    /* Temporary arrays: the maximum number of prime factors (with
     * multiplicity) of a 64‑bit integer is < 64. */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exponents[MAX_FACTORS];
    size_t prime_cnt = 0;

    /* Factor n.  factor_u64 expects the caller to supply arrays large
     * enough to hold the result.  The prime factors are returned in
     * ascending order. */
    factor_u64((uint64_t)n, primes, exponents, &prime_cnt);

    /* Build the ordered list of prime factors (with multiplicity). */
    uint64_t fact_list[MAX_FACTORS];
    size_t fact_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        for (size_t e = 0; e < exponents[i]; ++e) {
            fact_list[fact_cnt++] = primes[i];
        }
    }

    /* Compute the denominator of [0; p1, p2, ..., pk] via the
     * convergent recurrence.  q_-1 = 0, q_0 = 1. */
    seq_t q_prevprev = (seq_t)0;  /* q_{i-2} */
    seq_t q_prev     = (seq_t)1;  /* q_{i-1} */

    for (size_t i = 0; i < fact_cnt; ++i) {
        seq_t a_i = (seq_t)fact_list[i];
        seq_t q_curr = a_i * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }

    return q_prev;  /* q_k */
}

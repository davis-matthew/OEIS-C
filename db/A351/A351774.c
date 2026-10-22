/*
 * A351774  a(n) = Sum_{d|n} Sum_{p|n, p prime} d^p.
 * https://oeis.org/A351774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Sum_{p|n, p prime} d^p
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 25,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for n >= 1. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Small integer power routine using seq_t arithmetic. */
static seq_t my_pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;                /* a(1) = 0, a(0) undefined */

    /* Factor n into primes[] and exps[] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t total = 0;

    /* For each prime divisor p of n */
    for (size_t pi = 0; pi < cnt; ++pi) {
        uint64_t p = primes[pi];

        /* Compute product over all prime factors q of n:
           prod_q (1 + q^p + q^(2p) + ... + q^(e_q p))   */
        seq_t prod = 1;

        for (size_t qi = 0; qi < cnt; ++qi) {
            uint64_t q = primes[qi];
            uint64_t e = exps[qi];

            /* q^p once */
            seq_t q_pow_p = my_pow_uint(q, p);

            /* Geometric series sum for this q */
            seq_t term = 1;   /* current power q^(k*p) */
            seq_t sum_q = 1;  /* sum of 1 + q^p + ... */
            for (uint64_t j = 1; j <= e; ++j) {
                term *= q_pow_p;    /* q^(j*p) */
                sum_q += term;     /* add to geometric sum */
            }

            prod *= sum_q;          /* accumulate product */
        }

        total += prod;                /* accumulate over primes p */
    }

    return total;
}

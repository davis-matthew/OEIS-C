/*
 * A324506  a(n) = numerator of Product_{d|n} (d/tau(d)) where tau(k) = the
 *          number of divisors of k (A000005).
 * https://oeis.org/A324506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of ∏_{d|n} (d / τ(d))
 * Range:     u8 n <= 20, u16 n <= 41, u32 n <= 197, u64 n <= 419,
 *            u128 n <= 1889, big any n
 * Validated: standard, pass, 71 terms through n = 8317 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Count the exponent of prime p in the integer m (m >= 1). */
static idx_t exp_of_prime_in_uint64(uint64_t m, uint64_t p)
{
    idx_t cnt = 0;
    while (m % p == 0) {
        ++cnt;
        m /= p;
    }
    return cnt;
}

/* Sum of exponents of prime p in all integers from 1 to upto (inclusive). */
static idx_t sum_exp_in_range(idx_t upto, uint64_t p)
{
    idx_t sum = 0;
    for (idx_t k = 1; k <= upto; ++k) {
        sum += exp_of_prime_in_uint64(k, p);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* n == 0 is undefined; n == 1 gives 1 */
        return (seq_t)1;

    /* Factor n into primes. */
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &cnt);

    /* Compute τ(n) = Π (a_i + 1). */
    idx_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (idx_t)(exps_arr[i] + 1);

    seq_t result = (seq_t)1;

    /* For each prime divisor compute the surviving exponent e_i. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t  prime = primes_arr[i];
        idx_t     a_i   = (idx_t)exps_arr[i];

        /* Numerator exponent for this prime:  (τ * a_i) / 2. */
        idx_t N_exp = (tau * a_i) / 2;

        /* Denominator exponent: sum over all primes j. */
        idx_t D_exp = 0;
        for (size_t j = 0; j < cnt; ++j) {
            idx_t a_j   = (idx_t)exps_arr[j];
            idx_t t_j   = a_j + 1;
            idx_t mult  = tau / t_j;                         /* Π_{k≠j} t_k */
            idx_t T_j   = sum_exp_in_range(a_j + 1, prime);  /* Σ_{k=0}^{a_j} exp_p(k+1) */
            D_exp += mult * T_j;
        }

        if (N_exp > D_exp) {
            idx_t e = N_exp - D_exp;
            seq_t term = ipow_idx((seq_t)prime, e);
            result = seq_mul(result, term);
        }
    }

    return result;
}

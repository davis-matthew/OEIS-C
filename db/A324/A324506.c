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
 * Helpers:   exp_of_prime_in_uint64, factor_u64, seq_mul, seq_pow,
 *            sum_exp_in_range
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/exp_of_prime_in_uint64.h"
#include "../utils/sum_exp_in_range.h"

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
            seq_t term = seq_pow((seq_t)prime, e);
            result = seq_mul(result, term);
        }
    }

    return result;
}

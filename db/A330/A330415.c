/*
 * A330415  Coefficient of h(y) in Sum_{k > 0, i > 0} x_i^k = p(1) + p(2) +
 *          p(3) + ..., where h is the basis of homogeneous symmetric
 *          functions, p is the basis of power-sum symmetric functions, and y
 *          is the integer partition with Heinz number n.
 * https://oeis.org/A330415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(Ω(n)-1) * A056239(n) * (Ω(n)-1)! / ∏ c_i! , with
 *            Ω(n) = Σ e_i and A056239(n) = Σ e_i * π(p_i), π(p_i) = #primes ≤
 *            p_i
 * Range:     i8 n <= 47, i16 n <= 9999, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static size_t sieve_limit = 0;
static bool *is_prime_sieve = NULL;
static size_t *pi_prefix = NULL;

/* Ensure π(x) is available up to n, extending the sieve if needed */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit) return;

    /* Reallocate arrays to accommodate new limit */
    size_t new_limit = (size_t)n;
    bool *new_sieve = (bool*)malloc((new_limit + 1) * sizeof(bool));
    size_t *new_pi   = (size_t*)malloc((new_limit + 1) * sizeof(size_t));

    /* Initialize all entries as potentially prime */
    for (size_t i = 0; i <= new_limit; ++i) new_sieve[i] = true;
    if (new_limit >= 0) new_sieve[0] = false;
    if (new_limit >= 1) new_sieve[1] = false;

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i * i <= new_limit; ++i)
        if (new_sieve[i])
            for (size_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = false;

    /* Build π(x) prefix array */
    new_pi[0] = new_pi[1] = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        new_pi[i] = new_pi[i - 1] + (new_sieve[i] ? 1 : 0);

    /* Free old data */
    free(is_prime_sieve);
    free(pi_prefix);

    is_prime_sieve = new_sieve;
    pi_prefix      = new_pi;
    sieve_limit    = new_limit;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;               /* a(1) = 0 */

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Make sure π(p) is available for all primes in factorization */
    ensure_sieve(n);

    /* Compute Ω(n), product of factorials of exponents, and A056239(n) */
    size_t          omega = 0;
    seq_t           prod_exp_fact = 1;
    seq_t           sum_e_pi = 0;

    for (size_t i = 0; i < cnt; ++i) {
        omega += (size_t)exps[i];

        /* Factorial of exponent e_i */
        seq_t fact_e = 1;
        for (uint64_t j = 2; j <= exps[i]; ++j)
            fact_e *= (seq_t)j;
        prod_exp_fact *= fact_e;

        /* Sum e_i * π(p_i) */
        uint64_t p = primes[i];
        size_t pi_p = pi_prefix[p];
        sum_e_pi += (seq_t)exps[i] * (seq_t)pi_p;
    }

    /* (Ω-1)! */
    seq_t fact_omega_minus1 = 1;
    if (omega > 0) {
        for (size_t j = 2; j <= omega - 1; ++j)
            fact_omega_minus1 *= (seq_t)j;
    }

    /* Numerator: Σ e*π(p) * (Ω-1)! */
    seq_t numerator = sum_e_pi * fact_omega_minus1;

    /* Final result before sign */
    seq_t result = numerator / prod_exp_fact;

    /* Apply sign (-1)^(Ω-1) */
    if (((omega - 1) & 1) != 0)
        result = -result;

    return result;
}

/*
 * A336416  Number of perfect-power divisors of n!.
 * https://oeis.org/A336416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of perfect‑power divisors of n! (1 included)
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 97, u64 n <= 235,
 *            u128 n <= 565, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static inline uint64_t exponent_in_factorial(idx_t n, idx_t p) {
    uint64_t result = 0;
    idx_t t = n / p;
    while (t) {
        result += t;
        t /= p;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Special case n = 0 : 0! = 1, only divisor is 1 */
    if (n == 0)
        return (seq_t)1;

    /* ---- 1. Prime table up to n ------------------------------------ */
    uint64_t *p_list = NULL;
    size_t p_cnt = 0;
    build_primes_upto((idx_t)n, &p_list, &p_cnt);
    if (!p_list) return (seq_t)0;           /* allocation failure */

    /* ---- 2. Exponents of each prime in n! -------------------------- */
    uint32_t *exp = (uint32_t *)malloc(p_cnt * sizeof(uint32_t));
    if (!exp) {
        free(p_list);
        return (seq_t)0;
    }

    uint32_t e_max = 0;
    for (size_t i = 0; i < p_cnt; ++i) {
        idx_t p = p_list[i];
        uint64_t e = exponent_in_factorial(n, p);
        exp[i] = (uint32_t)e;
        if ((uint32_t)e > e_max)
            e_max = (uint32_t)e;
    }

    /* ---- 3. Total number of divisors of n! -------------------------- */
    seq_t total = 1;
    for (size_t i = 0; i < p_cnt; ++i)
        total *= (seq_t)(exp[i] + 1);

    /* ---- 4. Compute Möbius function up to e_max ------------------- */
    size_t mu_size = (size_t)e_max + 1;
    int *min_prime = (int *)calloc(mu_size, sizeof(int));
    int8_t *mu = (int8_t *)calloc(mu_size, sizeof(int8_t));
    uint32_t *mu_primes = (uint32_t *)malloc(mu_size / 2 * sizeof(uint32_t));
    size_t mu_prime_cnt = 0;
    if (!min_prime || !mu || !mu_primes) {
        free(p_list); free(exp); free(min_prime); free(mu); free(mu_primes);
        return (seq_t)0;
    }

    mu[1] = (int8_t)1;
    for (int i = 2; i <= (int)e_max; ++i) {
        if (min_prime[i] == 0) {
            min_prime[i] = i;
            mu[i] = (int8_t)-1;
            mu_primes[mu_prime_cnt++] = i;
        }
        for (size_t j = 0; j < mu_prime_cnt && (int)mu_primes[j] <= min_prime[i]
             && (long long)i * mu_primes[j] <= (long long)e_max; ++j) {
            int p = mu_primes[j];
            int ip = i * p;
            min_prime[ip] = p;
            if (i % p == 0)
                mu[ip] = 0;
            else
                mu[ip] = -mu[i];
        }
    }

    /* ---- 5. Accumulate sums of (A(k)-1) for k>=2 with μ(k) --------- */
    seq_t sum_neg = 0;   /* μ(k) = -1 */
    seq_t sum_pos = 0;   /* μ(k) =  1 */

    for (uint32_t k = 2; k <= e_max; ++k) {
        seq_t prod = 1;
        for (size_t i = 0; i < p_cnt; ++i) {
            uint64_t part = (uint64_t)exp[i] / k;
            prod *= (seq_t)(part + 1);
        }
        seq_t diff = prod - 1;   /* prod >= 1 */

        int8_t mk = mu[k];
        if (mk == -1)
            sum_neg += diff;
        else if (mk == 1)
            sum_pos += diff;
    }

    /* ---- 6. Final answer: 1 + sum_neg - sum_pos ------------------- */
    seq_t result = sum_neg;
    if (sum_pos <= result)
        result -= sum_pos;   /* sum_neg >= sum_pos for all n ≥ 0 */
    else
        result = 0;          /* safety, should not happen */

    result += 1;

    /* ---- 7. Cleanup ------------------------------------------------- */
    free(p_list);
    free(exp);
    free(min_prime);
    free(mu);
    free(mu_primes);

    return result;
}

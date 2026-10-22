/*
 * A336318  Square root of the largest square dividing n*d(n), where d(n) is
 *          the number of divisors of n, A000005.
 * https://oeis.org/A336318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = square root of the largest square dividing n*d(n)
 * Range:     u8 n <= 2903, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* generate all primes ≤ limit and return them in primes */
static void generate_primes(idx_t limit, idx_t **primes, size_t *cnt) {
    if (limit < 2) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p) is_prime[q] = false;
        }
    }
    *cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++(*cnt);
    *primes = (idx_t *)malloc(*cnt * sizeof(idx_t));
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) (*primes)[k++] = (idx_t)i;
    free(is_prime);
}

/* factor num into primes[] and exp[] up to primes_cnt primes, store count in cnt */
static void factor_uint64(uint64_t num, idx_t *primes, int *exp,
                          size_t *cnt, const idx_t *pr, size_t pr_cnt) {
    *cnt = 0;
    for (size_t i = 0; i < pr_cnt && (uint64_t)pr[i] * pr[i] <= num; ++i) {
        if (num % pr[i] == 0) {
            int e = 0;
            do {
                num /= pr[i];
                ++e;
            } while (num % pr[i] == 0);
            primes[*cnt] = pr[i];
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (num > 1) {
        primes[*cnt] = (idx_t)num;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* for safety, though n≥1 */
    idx_t *prime_list = NULL;
    size_t prime_cnt = 0;
    generate_primes(n, &prime_list, &prime_cnt);

    /* factor n */
    idx_t p_n[64];
    int exp_n[64];
    size_t cnt_n = 0;
    factor_uint64((uint64_t)n, p_n, exp_n, &cnt_n, prime_list, prime_cnt);

    /* compute d(n) */
    seq_t d_val = 1;
    for (size_t i = 0; i < cnt_n; ++i) d_val *= (seq_t)(exp_n[i] + 1);
    uint64_t d_val_u64 = (uint64_t)d_val;

    /* factor d(n) */
    idx_t p_d[64];
    int exp_d[64];
    size_t cnt_d = 0;
    factor_uint64(d_val_u64, p_d, exp_d, &cnt_d, prime_list, prime_cnt);

    /* merge exponents and take floor of half */
    idx_t p_res[64];
    int exp_res[64];
    size_t cnt_res = 0;

    for (size_t i = 0; i < cnt_n; ++i) {
        uint64_t p = p_n[i];
        int e_tot = exp_n[i];
        for (size_t j = 0; j < cnt_d; ++j) {
            if (p_d[j] == p) {
                e_tot += exp_d[j];
                break;
            }
        }
        int e_half = e_tot >> 1;           /* floor(e_tot/2) */
        if (e_half > 0) {
            p_res[cnt_res] = p;
            exp_res[cnt_res] = e_half;
            ++cnt_res;
        }
    }
    for (size_t j = 0; j < cnt_d; ++j) {
        uint64_t p = p_d[j];
        bool already = false;
        for (size_t i = 0; i < cnt_n; ++i) {
            if (p_n[i] == p) { already = true; break; }
        }
        if (!already) {
            int e_half = exp_d[j] >> 1;
            if (e_half > 0) {
                p_res[cnt_res] = p;
                exp_res[cnt_res] = e_half;
                ++cnt_res;
            }
        }
    }

    /* compute the product */
    seq_t result = 1;
    for (size_t i = 0; i < cnt_res; ++i) {
        seq_t base = (seq_t)p_res[i];
        seq_t power = 1;
        for (int k = 0; k < exp_res[i]; ++k) power *= base;
        result *= power;
    }

    free(prime_list);
    return result;
}

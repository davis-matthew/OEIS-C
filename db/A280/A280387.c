/*
 * A280387  Composite numbers n such that sum of proper divisors of n divides
 *          sum of proper divisors of n^n.
 * https://oeis.org/A280387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth composite number n such that 
 * sum_of_proper_divisors(n) divides sum_of_proper_divisors(n^n).
 * The function returns the nth term (a(1)=4, a(2)=8, ...).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

static seq_t sum_series(seq_t p, uint64_t k, seq_t m)
{
    /* Recursively compute sum_{i=0}^{k} p^i modulo m. */
    if (k == 0) return 1 % m;
    if (k == 1) return (1 + (p % m)) % m;

    if ((k & 1) == 0) {
        uint64_t half = k >> 1;
        seq_t sum_half, pow_half;
        /* Recursive call for half exponent */
        seq_t tmp_sum, tmp_pow;
        /* Compute sum_{0..half-1} and p^{half-1} */
        tmp_sum = sum_series(p, half - 1, m);
        tmp_pow = 1;
        for (uint64_t i = 0; i < half - 1; ++i) {
            tmp_pow = (tmp_pow * (p % m)) % m;
        }
        sum_half = tmp_sum;
        pow_half = tmp_pow;

        /* sum_{0..k} = sum_half + sum_half * p^{half} + p^k */
        seq_t pow_half_plus = (pow_half * (p % m)) % m;          /* p^{half}   */
        seq_t term = (sum_half + (sum_half * pow_half_plus) % m) % m;
        seq_t pow_k = (pow_half_plus * pow_half_plus) % m;       /* p^{k} = (p^{half})^2 */
        return (term + pow_k) % m;
    } else {
        seq_t prev = sum_series(p, k - 1, m);
        seq_t pow_k = mod_pow(p, k, m);
        return (prev + pow_k) % m;
    }
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t p = 3; p <= n / p; p += 2)
        if (n % p == 0) return false;
    return true;
}

static void factor_uint64(uint64_t n, uint64_t *pr, int *exp, size_t *cnt)
{
    *cnt = 0;
    uint64_t tmp = n;
    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        pr[(*cnt)++] = 2;
        exp[(*cnt) - 1] = e;
    }
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[(*cnt)++] = p;
            exp[(*cnt) - 1] = e;
        }
    }
    if (tmp > 1) {
        pr[(*cnt)++] = tmp;
        exp[(*cnt) - 1] = 1;
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_len = 0, term_cap = 0;
    static uint64_t candidate = 4;          /* start from first composite */

    /* Return already computed term if available */
    if (n <= term_len) return terms[n - 1];

    while (term_len < n) {
        /* Skip primes */
        while (is_prime_uint64(candidate)) candidate++;

        /* Factorise candidate */
        uint64_t primes[32];
        int exps[32];
        size_t fcnt;
        factor_uint64(candidate, primes, exps, &fcnt);

        /* Compute sigma(c) */
        seq_t sigma = 1;
        for (size_t i = 0; i < fcnt; ++i) {
            uint64_t p = primes[i];
            int e = exps[i];
            seq_t term = 1;
            seq_t pow = 1;
            for (int j = 0; j < e; ++j) {
                pow = pow * (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
        seq_t s_n = sigma - (seq_t)candidate;   /* sum of proper divisors */
        if (s_n == 0) { candidate++; continue; }

        seq_t mod = s_n;

        /* Compute sigma(c^c) mod mod */
        seq_t sigma_mod = 1 % mod;
        for (size_t i = 0; i < fcnt; ++i) {
            uint64_t p = primes[i];
            uint64_t a = exps[i];
            uint64_t k = a * candidate;          /* exponent in c^c */
            seq_t part = sum_series((seq_t)p, k, mod);
            sigma_mod = (sigma_mod * part) % mod;
        }

        /* n^n mod mod */
        seq_t n_pow_mod = mod_pow((seq_t)candidate, candidate, mod);

        /* s(c^c) mod mod */
        seq_t s_mod = sigma_mod >= n_pow_mod ?
                      sigma_mod - n_pow_mod : sigma_mod + mod - n_pow_mod;
        s_mod %= mod;

        if (s_mod == 0) {
            /* Store the term */
            if (term_len == term_cap) {
                term_cap = term_cap ? term_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_len++] = candidate;
        }
        ++candidate;
    }
    return terms[n - 1];
}

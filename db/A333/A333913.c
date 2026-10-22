/*
 * A333913  Numbers k such that lambda(k) is not the sum of 3 squares, where
 *          lambda is the Carmichael lambda function (A002322).
 * https://oeis.org/A333913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 8888, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_bad_sum_of_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333913:
   Numbers k such that λ(k) is of the form 4^a(8b+7) (i.e., λ(k) is NOT a sum of 3 squares). */

/* The code uses a simple sieve to generate primes up to 1,000,000,
   computes λ(k) by factorisation, tests the Legendre condition,
   caches all found terms, and answers arbitrary queries for a(n). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */
#include "../utils/is_bad_sum_of_three_squares.h"

/* --- Prime sieve (up to 1,000,000) --- */
#define PRIME_LIMIT 1000000
static unsigned char prime_mark[PRIME_LIMIT + 1];
static uint32_t primes[80000];
static int primes_count = 0;

static void init_primes(void) {
    if (primes_count) return;
    for (int i = 0; i <= PRIME_LIMIT; ++i) prime_mark[i] = 0;
    prime_mark[0] = prime_mark[1] = 1;
    for (int i = 2; (int64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!prime_mark[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                prime_mark[j] = 1;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!prime_mark[i]) primes[primes_count++] = i;
    }
}

/* --- Helpers for sequence terms cache --- */
static seq_t *cached_terms = NULL;
static size_t cached_len = 0;
static size_t cached_cap = 0;
static idx_t last_checked = 0;

static void append_term(seq_t val) {
    if (cached_len == cached_cap) {
        size_t new_cap = (cached_cap == 0) ? 16 : cached_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
        if (!new_arr) abort();   /* allocation failure */
        cached_terms = new_arr;
        cached_cap   = new_cap;
    }
    cached_terms[cached_len++] = val;
}

/* --- Compute Carmichael lambda for n --- */
static seq_t compute_lambda(idx_t n) {
    idx_t temp = n;
    seq_t result = 1;

    for (int i = 0; i < primes_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > (uint64_t)temp)
            break;
        if (temp % p == 0) {
            /* count exponent */
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            seq_t lambda_pow;
            if (p == 2) {
                if (exp == 1) lambda_pow = 1;
                else if (exp == 2) lambda_pow = 2;
                else {
                    seq_t pw = 1;
                    for (int k = 0; k < exp - 2; ++k) pw *= 2;
                    lambda_pow = pw;
                }
            } else {
                seq_t pw = 1;
                for (int k = 0; k < exp - 1; ++k) pw *= p;
                lambda_pow = pw * (p - 1);
            }

            seq_t g = gcd(result, lambda_pow);
            result = (result / g) * lambda_pow;
        }
    }

    if (temp > 1) {
        /* temp is a prime factor > sqrt(n) */
        seq_t lambda_pow;
        if (temp == 2) lambda_pow = 1;
        else lambda_pow = (seq_t)temp - 1;
        seq_t g = gcd(result, lambda_pow);
        result = (result / g) * lambda_pow;
    }

    return result;
}

/* --- Ensure cache has at least target terms --- */
static void compute_until(idx_t target) {
    if (target <= cached_len) return;
    init_primes();

    while (cached_len < target) {
        ++last_checked;
        seq_t lam = compute_lambda(last_checked);
        if (is_bad_sum_of_three_squares(lam)) {
            append_term((seq_t)last_checked);
        }
    }
}

/* --- Public sequence function --- */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but harmless */
    compute_until(n);
    return cached_terms[n - 1];
}

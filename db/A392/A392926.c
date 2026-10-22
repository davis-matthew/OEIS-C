/*
 * A392926  Numbers that are neither squarefree nor squareful, with at least 1
 *          repeated exponent of prime power factors.
 * https://oeis.org/A392926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  iterate integers, factorize with smallest‑prime‑factor (SPF)
   sieve, and collect those whose prime‑factor exponents satisfy:
        • at least three distinct primes;
        • at least one exponent equal to 1;
        • at least one exponent greater than 1;
        • at least one repeated exponent among the exponents.
   Each call to a(idx_t n) returns the nth term (1‑based).  The term
   values are memoised so that calls are O(1) after the initial
   search. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Memoised sequence terms */
static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* SPF table: smallest prime factor for each integer up to spf_limit */
static uint32_t *spf = NULL;
static uint32_t spf_limit = 0;

/* Recompute SPF table up to 'limit' (uses Eratosthenes sieve). */
static void recompute_spf(uint32_t limit)
{
    spf = (uint32_t *)realloc(spf,
                            (size_t)(limit + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= limit; ++i) {
        spf[i] = i;
    }
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                uint32_t idx = (uint32_t)j;
                if (spf[idx] == idx) {
                    spf[idx] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* Grow terms array to hold one more value. */
static void add_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    static uint32_t next_candidate = 2;  /* next integer to test */

    while (terms_len < (size_t)n) {
        /* Ensure SPF table covers next_candidate. */
        if ((uint64_t)next_candidate > spf_limit) {
            uint32_t new_limit = spf_limit ? spf_limit * 2 : 64;
            if (new_limit < next_candidate * 2)
                new_limit = (uint32_t)next_candidate * 2;
            recompute_spf(new_limit);
        }

        /* Factorize next_candidate using SPF. */
        uint32_t x = next_candidate;
        uint32_t temp = x;
        /* Exponents of the prime factors. */
        uint32_t exps[64];
        int expCnt = 0;

        while (temp > 1) {
            uint32_t p = spf[temp];
            uint32_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            exps[expCnt++] = cnt;
            if (expCnt >= 64) break;         /* safety, should not happen */
        }

        /* Check conditions: at least 3 distinct primes, one exponent 1,
           one exponent >1, and at least one repeated exponent. */
        if (expCnt >= 3) {
            bool hasOne = false, hasMoreOne = false;
            for (int i = 0; i < expCnt; ++i) {
                if (exps[i] == 1) hasOne = true;
                if (exps[i] > 1) hasMoreOne = true;
            }
            if (hasOne && hasMoreOne) {
                bool repeated = false;
                for (int i = 0; !repeated && i < expCnt - 1; ++i) {
                    for (int j = i + 1; j < expCnt; ++j) {
                        if (exps[i] == exps[j]) {
                            repeated = true;
                            break;
                        }
                    }
                }
                if (repeated) add_term((seq_t)x);
            }
        }

        ++next_candidate;
    }
    return terms[(size_t)n - 1];
}

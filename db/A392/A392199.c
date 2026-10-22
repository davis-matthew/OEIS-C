/*
 * A392199  Positive integers k such that gcd(k, sigma(k)) is prime.
 * https://oeis.org/A392199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate integers k such that gcd(k, sigma(k)) is prime by
   iteratively testing increasing numbers and caching results. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;
static size_t terms_cap = 0, terms_used = 0;
static uint64_t next_k = 2;

static uint64_t *spf = NULL;
static size_t spf_cap = 0;

static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);
    terms = new_arr;
    terms_cap = new_cap;
}

static void recompute_spf(size_t limit)
{
    uint64_t *new_spf = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_spf) exit(1);
    memset(new_spf, 0, (limit + 1) * sizeof(uint64_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint64_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint64_t)i;
                }
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_cap = limit + 1;
}

static void ensure_spf(size_t up_to)
{
    if (up_to < spf_cap) return;
    recompute_spf(up_to);
}

static seq_t sigma_u64(uint64_t n)
{
    seq_t sigma = 1;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = spf[m];
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t sum_p = 1;
        seq_t term = 1;
        for (uint64_t i = 0; i < exp; ++i) {
            term *= (seq_t)p;
            sum_p += term;
        }
        sigma *= sum_p;
    }
    return sigma;
}

static void compute_terms_up_to(idx_t target)
{
    while (terms_used < (size_t)target) {
        uint64_t k = next_k++;
        ensure_spf(k);
        seq_t sigma = sigma_u64(k);
        seq_t g = gcd((seq_t)k, sigma);
        if (g > (seq_t)1 && is_prime_big(g)) {
            ensure_terms_capacity(terms_used + 1);
            terms[terms_used++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_terms_up_to(n);
    return terms[(size_t)n - 1];
}

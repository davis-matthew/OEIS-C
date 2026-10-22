/*
 * A368474  Product of exponents of prime factorization of the numbers whose
 *          exponents in their prime power factorization are squares
 *          (A197680).
 * https://oeis.org/A368474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of the exponents in the prime factorisation * of
 *            the numbers whose exponents are all perfect squares. * This
 *            sequence orders such numbers in increasing order and returns the
 *            * corresponding product of exponents
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exp_is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/exp_is_square.h"

/* ----------  Smallest‑prime‑factor sieve  ---------- */
static uint32_t *spf = NULL;      /* spf[i] = least prime factor of i */
static idx_t    spf_limit = 0;    /* size of the sieve */

/* Build the sieve up to “limit” (inclusive). */
static void build_spf(idx_t limit)
{
    if (spf) free(spf);
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure the sieve covers at least “require”. */
static void ensure_spf(idx_t require)
{
    if (spf_limit >= require) return;
    idx_t new_limit = spf_limit == 0 ? 2 : spf_limit;
    while (new_limit < require) new_limit <<= 1;
    build_spf(new_limit);
}

/* ----------  Cache for produced terms  ---------- */
static seq_t *term_cache = NULL;   /* a(1)…a(k) */
static size_t cache_len  = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* current capacity of term_cache */
static idx_t   last_checked = 0;   /* largest integer that has been examined */

/* Grow the cache array to accommodate at least “need” terms. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap < 16 ? 16 : cache_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    term_cache = new_buf;
    cache_cap  = new_cap;
}

/* ----------  Main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined only for n ≥ 1 */

    /* Extend cache until it contains at least n terms. */
    if (cache_len < (size_t)n) {
        grow_cache((size_t)n);

        idx_t num = last_checked + 1;
        for (; cache_len < (size_t)n; ++num) {
            /* Ensure spf covers this number. */
            ensure_spf(num);

            bool ok = true;
            seq_t prod = (seq_t)1;
            idx_t temp = num;

            if (temp == 1) {
                /* By definition the product for 1 is 1. */
                ok = true;
            } else {
                while (temp > 1) {
                    uint32_t p = spf[temp];
                    int exp = 0;
                    while (temp % p == 0) {
                        temp /= p;
                        ++exp;
                    }
                    if (!exp_is_square(exp)) { ok = false; break; }
                    prod *= (seq_t)exp;
                }
            }

            if (ok) {
                term_cache[cache_len++] = prod;
            }

            last_checked = num;   /* record that we have examined this number */
        }
    }
    return term_cache[n - 1];
}

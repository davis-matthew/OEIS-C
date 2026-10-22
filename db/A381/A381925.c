/*
 * A381925  Positive integers k which have at least one divisor d for which
 *          tau(k) = sigma(d).
 * https://oeis.org/A381925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: positive integer k which has a divisor d with τ(k) = σ(d) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* tables for smallest prime factor, divisor count τ, and σ */
static uint32_t *g_spf   = NULL; /* smallest prime factor */
static uint32_t *g_tau   = NULL; /* divisor count τ(k) */
static seq_t   *g_sigma = NULL; /* sum of divisors σ(k) */
static size_t   g_limit  = 0;    /* largest k for which tables are valid */

/* rebuild tables up to 'limit' */
static void build_tables(size_t limit)
{
    size_t i, j;
    free(g_spf); free(g_tau); free(g_sigma);
    g_spf   = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    g_tau   = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    g_sigma = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!g_spf || !g_tau || !g_sigma) abort();

    for (i = 0; i <= limit; ++i) g_spf[i] = 0;
    if (limit >= 1) g_spf[1] = 1;
    for (i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (j = i * i; j <= limit; j += i)
                    if (g_spf[j] == 0) g_spf[j] = (uint32_t)i;
            }
        }
    }

    for (i = 1; i <= limit; ++i) {
        g_tau[i]   = 0;
        g_sigma[i] = 0;
    }
    for (i = 1; i <= limit; ++i) {
        uint32_t v = (uint32_t)i;
        for (j = i; j <= limit; j += i) {
            ++g_tau[j];
            g_sigma[j] += (seq_t)v;
        }
    }
}

/* recursive divisor enumeration */
static bool dfs_div(int idx, uint64_t cur,
                    const uint32_t *primes, const uint8_t *exps,
                    int total, uint32_t tau_k)
{
    if (idx == total) {
        return g_sigma[(size_t)cur] == tau_k;
    }
    uint32_t p = primes[idx];
    uint8_t  e = exps[idx];
    uint64_t mult = 1;
    for (uint8_t k = 0; k <= e; ++k) {
        if (dfs_div(idx + 1, cur * mult, primes, exps, total, tau_k))
            return true;
        mult *= p;
    }
    return false;
}

/* check whether k has a divisor d with τ(k)=σ(d) */
static bool has_special_divisor(uint64_t k, uint32_t tau_k)
{
    uint32_t primes[10];
    uint8_t  exps[10];
    int total = 0;
    uint64_t tmp = k;
    while (tmp > 1) {
        uint32_t p = g_spf[(size_t)tmp];
        uint8_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        primes[total] = p;
        exps[total]   = cnt;
        ++total;
        if (total >= 10) break;   /* safety guard */
    }
    return dfs_div(0, 1, primes, exps, total, tau_k);
}

/* storage for generated sequence values */
static seq_t   *g_terms = NULL;
static size_t  g_terms_cap = 0;
static size_t  g_terms_len = 0;
static uint64_t g_last_k   = 0;

seq_t a(idx_t n)
{
    if (g_limit == 0) {
        size_t init = 1024;
        g_limit = init;
        build_tables(init);
    }

    while (g_terms_len < n) {
        uint64_t k = g_last_k + 1;
        if (k > g_limit) {
            size_t new_limit = g_limit * 2;
            if (new_limit < (size_t)k) new_limit = (size_t)k;
            build_tables(new_limit);
            g_limit = new_limit;
        }
        uint32_t tau_k = g_tau[(size_t)k];
        if (has_special_divisor(k, tau_k)) {
            if (g_terms_len == g_terms_cap) {
                size_t new_cap = g_terms_cap ? g_terms_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(g_terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                g_terms = tmp;
                g_terms_cap = new_cap;
            }
            g_terms[g_terms_len++] = (seq_t)k;
        }
        ++g_last_k;
    }

    return g_terms[n - 1];
}

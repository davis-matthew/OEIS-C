/*
 * A343217  Numbers k such that A003415(sigma(k)) >= k, where A003415(x) gives
 *          the arithmetic derivative of x.
 * https://oeis.org/A343217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, pow_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that A003415(sigma(k)) >= k.  The function returns the n‑th such number. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/pow_local.h"

static uint64_t *spf = NULL;
static idx_t spf_cap = 0;

/* Build the smallest prime factor table up to limit (inclusive). */
static void build_spf(idx_t limit)
{
    if (limit <= spf_cap) return;

    uint64_t *new_spf = (uint64_t*)calloc((size_t)limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(1);
    if (spf) free(spf);
    spf = new_spf;

    /* Linear sieve */
    static uint64_t *pr = NULL;
    static size_t pr_cap = 0, pr_len = 0;
    if ((size_t)limit + 1 > pr_cap) {
        uint64_t *tmp = (uint64_t*)realloc(pr, ((size_t)limit + 1) * sizeof(uint64_t));
        if (!tmp) exit(1);
        pr = tmp;
        pr_cap = (size_t)limit + 1;
    }
    pr_len = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (pr_len < pr_cap) pr[pr_len++] = i;
        }
        for (size_t j = 0; j < pr_len; ++j) {
            uint64_t p = pr[j];
            if (p > spf[i] || (uint64_t)i * p > limit) break;
            spf[i * p] = p;
        }
    }
    spf_cap = limit;
}

/* Cache for the sequence values. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t processed_k = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1: value for n=0 is undefined */

    /* Ensure a sufficiently large spf table at start. */
    if (spf_cap == 0) build_spf(2000000);

    while (cache_len < (size_t)n) {
        ++processed_k;
        uint64_t k = (uint64_t)processed_k;

        /* Compute σ(k). */
        seq_t sigma = 1;
        uint64_t t = k;
        while (t > 1) {
            uint64_t p = spf[t];
            if (p == 0) p = t;
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }

            seq_t term = pow_local((seq_t)p, e + 1) - 1;
            term /= ((seq_t)p - 1);
            sigma *= term;
        }

        /* Compute D(σ(k)) using its factorisation. */
        uint64_t sigma_val = (uint64_t)sigma;   /* fits in 64‑bit for our limits */
        if (sigma_val > spf_cap) build_spf(sigma_val);

        seq_t derivative = 0;
        uint64_t u = sigma_val;
        while (u > 1) {
            uint64_t p = spf[u];
            if (p == 0) p = u;
            uint64_t e = 0;
            while (u % p == 0) { u /= p; ++e; }

            seq_t part = sigma / (seq_t)p;
            part *= e;
            derivative += part;
        }

        if (derivative >= (seq_t)processed_k)
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)processed_k);
    }

    return cache[n - 1];
}

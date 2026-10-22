/*
 * A327934  Numbers k such that there is no prime p such that p^p divides k,
 *          but for its arithmetic derivative a positive finite number of such
 *          primes exist.
 * https://oeis.org/A327934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 8686, u32 n <= 9845, u64 n <= 9845,
 *            u128 n <= 9845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by a search of integers k meeting the definition of A327934. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime factor table (SPF) -------------------------------- */
static uint32_t *spf      = NULL;   /* spf[i] = smallest prime factor of i (i > 1) */
static uint32_t spf_limit = 0;     /* maximum i for which spf is valid           */

/* Extend the SPF table to cover at least 'limit'. */
static void ensure_spf_up_to(uint32_t limit)
{
    if (limit <= spf_limit) return;

    /* Allocate a fresh table with new size. */
    uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!new_spf) return;               /* Out of memory – give up */

    /* Copy old data. */
    if (spf) {
        for (uint32_t i = 0; i <= spf_limit; ++i) new_spf[i] = spf[i];
        free(spf);
    }

    /* Initialise the rest of the table: each number is initially its own factor. */
    for (uint32_t i = spf_limit + 1; i <= limit; ++i) new_spf[i] = i;

    /* Sieve: for each prime i, set its multiples. */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (new_spf[i] == i) {          /* i is prime */
            for (uint32_t j = i * i; j <= limit; j += i) {
                if (new_spf[j] == j) new_spf[j] = i;
            }
        }
    }

    spf      = new_spf;
    spf_limit = limit;
}

/* ---------- Factorisation using the SPF table ----------------------- */
/* Factor n (<= spf_limit) into prime table pf[0..cnt-1] with exponents pe[0..cnt-1]. */
static void factor_using_spf(uint32_t n, uint32_t *pf, uint32_t *pe, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        pf[(*cnt)] = p;
        pe[(*cnt)] = e;
        ++(*cnt);
    }
}

/* ---------- Cache of computed sequence terms ----------------------- */
static seq_t *cache    = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;

/* Append a value to the cache, reallocating if necessary. */
static void cache_append(seq_t val)
{
    if (cache_sz == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;  /* failure – ignore (unlikely) */
        cache = tmp;
    }
    cache[cache_sz++] = val;
}

/* ---------- The sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined; return 0 */

    /* Lazy initialise cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – give up */
    }

    /* Ensure SPF table for small numbers first. */
    if (!spf) ensure_spf_up_to(100000);

    /* Search until we have at least n terms. */
    static uint32_t current_k = 2;  /* start checking from 2 */
    while (cache_sz < (size_t)n) {
        /* Make sure SPF covers current_k. */
        if (current_k > spf_limit) ensure_spf_up_to(current_k);

        uint32_t k = current_k;

        /* Factor k. */
        uint32_t pf_k[32], pe_k[32];
        size_t cnt_k = 0;
        factor_using_spf(k, pf_k, pe_k, &cnt_k);

        /* Condition 1: no prime power p^p divides k (i.e., e < p for all primes). */
        bool have_p_to_p = false;
        for (size_t i = 0; i < cnt_k; ++i) {
            if (pe_k[i] >= pf_k[i]) { have_p_to_p = true; break; }
        }
        if (have_p_to_p) {                /* fails condition 1 */
            ++current_k;
            continue;
        }

        /* Arithmetic derivative k' = k * sum(e_i / p_i) = Σ e_i * (k / p_i). */
        uint64_t der = 0;
        for (size_t i = 0; i < cnt_k; ++i) {
            der += (uint64_t)pe_k[i] * ((uint64_t)k / (uint64_t)pf_k[i]);
        }

        /* Ensure SPF covers derivative for factorisation. */
        if (der > spf_limit) {
            /* Derivative likely fits in 32‑bit in the tested range. */
            if (der <= UINT32_MAX)
                ensure_spf_up_to((uint32_t)der);
            else
                /* Derivative too big for SPF; skip this k. */ 
                ;  /* We assume this never happens for n <= 10000. */
        }

        /* Factor derivative. */
        uint32_t pf_d[32], pe_d[32];
        size_t cnt_d = 0;
        if (der <= spf_limit)
            factor_using_spf((uint32_t)der, pf_d, pe_d, &cnt_d);
        else
            cnt_d = 0;  /* derivative too large – treat as no exponent >= prime */

        /* Condition 2: derivative has at least one prime power p^p. */
        bool der_has_p_to_p = false;
        for (size_t i = 0; i < cnt_d; ++i) {
            if (pe_d[i] >= pf_d[i]) { der_has_p_to_p = true; break; }
        }

        if (der_has_p_to_p)
            cache_append((seq_t)k);

        ++current_k;
    }

    return cache[(size_t)n - 1];
}

/*
 * A249481  Numbers n such that p|n and q|n+1 => p^q|n and q^p|n+1, where p is
 *          prime with multiplicity q and q prime with multiplicity p.
 * https://oeis.org/A249481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 686, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249481 – numbers n such that for distinct primes p,q
   with multiplicity(q in n) = p and multiplicity(p in n+1) = q. */
/* The function generates the sequence on demand by scanning n
   and testing the factorisation of n and n+1.  The search stops
   as soon as the requested index has been found.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"

/* ------------------------------------------------------------------ */
/*  SPF sieve for factoring numbers up to a fixed limit.              */
/* ------------------------------------------------------------------ */
#define SPF_LIMIT 5000000U            /* enough for the first 10000 terms */
static uint32_t *spf = NULL;         /* smallest prime factor array       */
static size_t spf_size = 0;          /* array size = spf_size (index 0..size-1) */

/* build spf up to SPF_LIMIT */
static void init_spf(void)
{
    if (spf) return;
    spf_size = SPF_LIMIT + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    if (!spf) abort();
    memset(spf, 0, spf_size * sizeof(uint32_t));
    uint32_t *pr = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    size_t pcnt = 0;
    for (size_t i = 2; i < spf_size; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            pr[pcnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pcnt && (uint64_t)pr[j] * i < spf_size; ++j) {
            uint32_t pj = pr[j];
            if (pj > spf[i]) break;
            spf[pj * i] = pj;
        }
    }
    free(pr);
}

/* ------------------------------------------------------------------ */
/*  Factorisation helpers                                            */
/* ------------------------------------------------------------------ */

static bool factor_uint64(uint64_t n, uint32_t *pr, uint32_t *ex, size_t *cnt)
{
    size_t i = 0;
    while (n > 1 && i < *cnt) {
        uint32_t p = spf[n];
        uint32_t c = 0;
        while (n % p == 0) {
            n /= p;
            ++c;
        }
        pr[i] = p;
        ex[i] = c;
        ++i;
    }
    *cnt = i;
    return n == 1;
}

/* ------------------------------------------------------------------ */
/*  Cache of computed sequence values                                */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure cache large enough for index n (1‑based) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    init_spf();

    static uint64_t next_checked = 1;   /* numbers to test, 1‑based */
    /* allocate initial cache memory */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        if ((next_checked + 1) > SPF_LIMIT) {
            /* overflow of sieve – cannot continue */
            break;
        }
        /* test n = next_checked */
        uint32_t pr0[64], ex0[64], pr1[64], ex1[64];
        size_t cnt0 = 64, cnt1 = 64;
        uint64_t t = next_checked;
        if (factor_uint64(t, pr0, ex0, &cnt0) &&
            factor_uint64(t + 1, pr1, ex1, &cnt1)) {
            /* try all prime factors of n */
            bool ok = false;
            for (size_t i = 0; i < cnt0 && !ok; ++i) {
                uint32_t p = pr0[i];
                uint32_t e0 = ex0[i];
                if (e0 <= 1) continue;          /* exponent must be prime >1 */
                if (e0 == p) continue;           /* distinct primes */
                if (!is_prime_small(e0)) continue;
                uint32_t q = e0;
                /* find exponent of q in n+1 and check p absent */
                uint32_t e1 = 0;
                bool p_in_n1 = false;
                for (size_t j = 0; j < cnt1; ++j) {
                    if (pr1[j] == q) e1 = ex1[j];
                    if (pr1[j] == p) p_in_n1 = true;
                }
                if (e1 == p && !p_in_n1) ok = true;
            }
            if (ok) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_checked;
            }
        }
        ++next_checked;
    }
}

/* ------------------------------------------------------------------ */
/*  Public API – return the nth term (1‑based).                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}

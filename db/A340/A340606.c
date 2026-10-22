/*
 * A340606  Numbers whose prime indices (A112798) are all divisors of the
 *          number of prime factors (A001222).
 * https://oeis.org/A340606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: nth positive integer whose prime indices all divide Ω(n)
   (A001222).  The function generates numbers in increasing order, tests
   the divisibility property using a dynamic smallest‑prime‑factor sieve,
   and caches the found terms.  It works for all provided builds
   (fixed‑width or arbitrary‑precision seq_t). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* global data for term cache and prime sieve                            */
static seq_t *g_terms      = NULL;   /* sequence values                 */
static size_t g_len        = 0;       /* number of values               */
static size_t g_cap        = 0;       /* allocated slots                */
static idx_t  g_next_test  = 2;       /* next integer to test           */
static idx_t  g_limit      = 2000000; /* current sieve limit            */
static bool   g_sieve_built = false; /* sieve has been built           */

static uint32_t *g_spf        = NULL; /* smallest prime factor map      */
static uint32_t *g_idx_prime  = NULL; /* prime index array              */

/* --------------------------------------------------------------------- */
/* build a linear sieve (spf and prime indices) up to `limit`            */
static void build_sieve(idx_t limit)
{
    /* free previous data */
    free(g_spf);
    free(g_idx_prime);

    /* allocate fresh arrays (cast for C++ compatibility) */
    g_spf = (uint32_t*)calloc((size_t)(limit) + 1, sizeof(uint32_t));
    g_idx_prime = (uint32_t*)calloc((size_t)(limit) + 1, sizeof(uint32_t));
    if (!g_spf || !g_idx_prime) {
        exit(EXIT_FAILURE);
    }

    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {                     /* i is prime          */
            g_spf[i] = (uint32_t)i;
            ++prime_cnt;
            g_idx_prime[i] = (uint32_t)prime_cnt;
            /* mark composites (first visit gets the smallest p) */
            for (idx_t j = i + i; j <= limit; j += i) {
                if (g_spf[j] == 0) {
                    g_spf[j] = (uint32_t)i;
                }
            }
        }
    }
    g_limit = limit;
    g_sieve_built = true;
}

/* --------------------------------------------------------------------- */
/* test whether a number `v` satisfies the property                      */
static bool is_valid(uint64_t v)
{
    if (v == 1) return true;          /* 1 is always valid */

    if (v > g_limit) return false;    /* safety guard */

    /* factorise. local array for indices of each prime factor occurrence */
    uint32_t prime_indices[128];      /* ample for Ω up to a few hundred */
    size_t idx_cnt = 0;
    uint64_t tmp = v;

    while (tmp > 1) {
        uint32_t p = g_spf[(size_t)tmp];
        uint32_t idx = g_idx_prime[p];
        /* push as many times as p repeats */
        do {
            prime_indices[idx_cnt++] = idx;
        } while ((tmp /= p) % p == 0);
    }

    uint64_t omega = idx_cnt;   /* Ω(v) */
    for (size_t i = 0; i < idx_cnt; ++i) {
        if (omega % prime_indices[i] != 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* ensure the cache contains at least `n-1` values for n>1                */
static void ensure_terms(idx_t n)
{
    if (n <= 1) return;          /* only a(1) = 1 */

    /* lazy sieve initialisation */
    if (!g_sieve_built) {
        build_sieve(g_limit);
    }

    idx_t needed = n - 1;                     /* terms >1 needed   */
    while ((idx_t)g_len < needed) {
        /* if we ran out of sieve range, expand it */
        if (g_next_test > g_limit) {
            idx_t new_limit = g_limit * 2 + 100;
            build_sieve(new_limit);
        }

        if (is_valid(g_next_test)) {
            /* grow term cache if necessary */
            if (g_len == g_cap) {
                size_t new_cap = (g_cap == 0) ? 16 : g_cap * 2;
                seq_t *tmp = (seq_t*)realloc(g_terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                g_terms = tmp;
                g_cap = new_cap;
            }
            g_terms[g_len++] = (seq_t)g_next_test;
        }
        ++g_next_test;
    }
}

/* --------------------------------------------------------------------- */
/* public sequence function                                               */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    ensure_terms(n);
    return g_terms[n - 2];           /* a(2) is g_terms[0] */
}

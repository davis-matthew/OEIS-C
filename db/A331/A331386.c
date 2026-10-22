/*
 * A331386  Numbers with at least one prime prime index.
 * https://oeis.org/A331386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) consists of all integers that have a prime divisor whose
 *            1‑based prime index is itself a prime. This implementation
 *            generates primes up to a fixed limit, pre‑computes the primes
 *            whose index is prime, and then builds the sequence lazily
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper functions used below are provided by the test harness.
   They are declared in the following headers. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes(limit, &arr, &cnt) */
#include "../utils/is_prime.h"       /* is_prime(idx_t) */

static bool initialized = false;

static const idx_t PRIMES_LIMIT = 200000;   /* plenty for n <= 10000 */

static uint64_t *all_primes = NULL;   /* array of primes up to PRIMES_LIMIT */
static size_t prime_cnt   = 0;

static seq_t *special_primes = NULL; /* primes whose 1‑based index is prime */
static size_t special_cnt    = 0;

static seq_t *term_cache = NULL;      /* cached terms of the sequence */
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t next_candidate = 3;       /* candidate integers to test */

/* ------------------------------------------------------------------ */
/* initialise static data (generated once)                            */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* ------------------------------------------------------------------ */
    /* generate all primes up to PRIMES_LIMIT                                */
    /* ------------------------------------------------------------------ */
    build_primes(PRIMES_LIMIT, (idx_t **)&all_primes, &prime_cnt);

    /* ------------------------------------------------------------------ */
    /* collect the primes whose 1‑based index is prime (prime indices)     */
    /* ------------------------------------------------------------------ */
    special_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (is_prime((idx_t)(i + 1))) ++special_cnt;
    }
    if (special_cnt > 0) {
        special_primes = (seq_t *)malloc(special_cnt * sizeof(seq_t));
        size_t k = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (is_prime((idx_t)(i + 1))) {
                special_primes[k++] = (seq_t)all_primes[i];
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /* allocate initial cache for sequence terms                            */
    /* ------------------------------------------------------------------ */
    cache_cap   = 128;
    term_cache  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len   = 0;
    next_candidate = 3; /* first odd candidate to test */
}

/* ---------------------------------------------------------------------- */
/* Check whether cand (as an integer) has a prime divisor whose index is prime */
/* ---------------------------------------------------------------------- */
static bool has_special_prime_factor(idx_t cand)
{
    /* We only need to check special primes up to cand. */
    for (size_t i = 0; i < special_cnt; ++i) {
        seq_t p = special_primes[i];
        if ((seq_t)cand < p) break;
        if ((cand % (idx_t)p) == 0) return true;
    }
    return false;
}

/* ----------------------------------------------------------------- */
/* Return the nth term of the sequence (n >= 1).                     */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for misuse */

    init_sequence();

    /* Keep generating terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t cand = next_candidate++;

        /* skip numbers that cannot possibly qualify   */
        if (cand < 3) continue;

        if (has_special_prime_factor(cand)) {
            /* grow cache if needed */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                term_cache = (seq_t *)realloc(term_cache,
                                 cache_cap * sizeof(seq_t));
            }
            term_cache[cache_len++] = (seq_t)cand;
        }
    }

    return term_cache[(size_t)n - 1];
}

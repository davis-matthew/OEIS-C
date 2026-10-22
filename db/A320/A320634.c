/*
 * A320634  Odd numbers whose multiset multisystem is a multiset partition
 *          spanning an initial interval of positive integers (odd = no empty
 *          sets).
 * https://oeis.org/A320634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A320634: odd numbers whose multiset multisystem is a
   multiset partition spanning an initial interval of positive integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 2000000            /* upper bound for sieve (must cover required values) */
#define MAX_PRIMES 200000        /* enough to hold all primes <= LIMIT */

static uint32_t spf[LIMIT + 1];          /* smallest prime factor */
static uint32_t primes[MAX_PRIMES];      /* list of primes up to LIMIT */
static size_t   prime_cnt = 0;           /* number of primes */
static uint32_t prime_index[LIMIT + 1];  /* prime_index[p] = 1‑based index of prime p */
static uint32_t stamp_seen[LIMIT + 1];   /* timestamp array for distinct-set marking */
static uint32_t stamp_counter = 1;       /* global time stamp, never zero   */

/* ------------------------------------------------------------------ */
/* Linear sieve : fills spf, primes, prime_index arrays               */
static void init_sieve(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    for (uint32_t i = 2; i <= LIMIT; ++i) spf[i] = 0;

    size_t pc = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (pc < MAX_PRIMES) primes[pc++] = i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (p > spf[i] || ip > LIMIT) break;
            spf[ip] = p;
        }
    }
    prime_cnt = pc;

    /* fill prime_index */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        prime_index[p] = (uint32_t)(i + 1); /* 1‑based */
    }
    /* initialise stamp_seen to 0 */
    for (size_t i = 0; i <= prime_cnt; ++i) stamp_seen[i] = 0;
}

/* ------------------------------------------------------------------ */
/* Test whether odd n satisfies the multiset‑partition property     */
static bool passes(uint64_t n)
{
    if (n == 1) return true;

    uint32_t stamp = stamp_counter++;
    if (stamp == 0) {          /* overflow guard – very unlikely */
        stamp = 1;
        stamp_counter = 2;
        for (size_t i = 0; i <= prime_cnt; ++i) stamp_seen[i] = 0;
    }

    uint32_t distinct = 0;
    uint32_t max_val = 0;

    uint64_t temp = n;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t exp = 0;
        while (temp % p == 0) { temp /= p; exp++; }

        uint32_t idx_p = prime_index[p];        /* prime index of p (1‑based) */

        /* factor idx_p once – duplicates contribute no new distinct values */
        uint32_t idx_temp = idx_p;
        while (idx_temp > 1) {
            uint32_t q = spf[idx_temp];
            while (idx_temp % q == 0) { idx_temp /= q; }
            uint32_t idx_q = prime_index[q];    /* prime index of q */

            if (stamp_seen[idx_q] != stamp) {
                stamp_seen[idx_q] = stamp;
                distinct++;
                if (idx_q > max_val) max_val = idx_q;
            }
        }
    }

    return (distinct > 0) && (max_val == distinct);
}

/* ------------------------------------------------------------------ */
/* Cache for previously computed terms                                 */
static seq_t *term_cache = NULL;   /* array of terms */
static size_t  cache_size = 0;     /* number of cached terms      */
static uint64_t next_candidate = 3;/* next odd integer to test    */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_sieve();

    /* ensure at least the first term */
    if (cache_size == 0) {
        term_cache = (seq_t*)malloc(sizeof(seq_t));
        term_cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* generate terms up to index n */
    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* skip even numbers */

        if (!passes(cand)) continue;  /* not a valid term */

        /* append to cache */
        seq_t *tmp = (seq_t*)realloc(term_cache, (cache_size + 1) * sizeof(seq_t));
        if (!tmp) abort();            /* memory failure, terminate */
        term_cache = tmp;
        term_cache[cache_size++] = (seq_t)cand;
    }

    return term_cache[n - 1];
}

/*
 * A365521  a(1) = 1; for n > 1, a(n) is the prime factor of n that has not
 *          appeared for the longest time in {a(1),...,a(n-2),a(n-1)}.
 * https://oeis.org/A365521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence of sequence A365521:
   a(1) = 1,
   for n > 1, a(n) is the prime factor of n that has not appeared
   for the longest time in a(1), …, a(n‑1).  The largest unused prime
   factor is chosen. */
/* Implementation uses memoisation and a linear‑time smallest‑prime‑factor sieve. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static memoisation tables ---------- */
static seq_t *seq_cache = NULL;   /* 1‑based a-values, seq_cache[0] unused   */
static size_t cache_cap = 0;      /* allocated size of seq_cache             */
static idx_t cur_max = 0;         /* highest n that has already been computed */

static uint32_t *spf = NULL;      /* smallest prime factor for each i        */
static size_t   spf_cap = 0;      /* allocated size of spf                  */

static idx_t *last_occ = NULL;    /* last index where each prime appeared    */
static size_t last_occ_cap = 0;   /* allocated size of last_occ             */

/* ---------- helper: rebuild spf array up to 'limit' ---------- */
static void rebuild_spf(idx_t limit)
{
    if (limit < 2) return;

    if (spf_cap < (size_t)(limit + 1)) {
        uint32_t *tmp = (uint32_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        spf = tmp;
        spf_cap = (size_t)(limit + 1);
    }

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * (idx_t)i <= limit; ++i) {
        if (spf[i] == 0) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    /* remaining primes */
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint32_t)i;
}

/* ---------- helper: ensure all buffers cover 'n' ---------- */
static void ensure_capacity(idx_t n)
{
    /* seq_cache */
    if (seq_cache == NULL) {
        seq_cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        cache_cap = (size_t)(n + 1);
    } else if (cache_cap < (size_t)(n + 1)) {
        seq_cache = (seq_t *)realloc(seq_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!seq_cache) exit(EXIT_FAILURE);
        cache_cap = (size_t)(n + 1);
    }

    /* last_occ */
    if (last_occ == NULL) {
        last_occ = (idx_t *)calloc((size_t)(n + 1), sizeof(idx_t));
        if (!last_occ) exit(EXIT_FAILURE);
        last_occ_cap = (size_t)(n + 1);
    } else if (last_occ_cap < (size_t)(n + 1)) {
        last_occ = (idx_t *)realloc(last_occ, (size_t)(n + 1) * sizeof(idx_t));
        if (!last_occ) exit(EXIT_FAILURE);
        last_occ_cap = (size_t)(n + 1);
    }

    /* spf */
    if (spf_cap < (size_t)(n + 1)) rebuild_spf(n);
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, safety */

    ensure_capacity(n);

    /* compute values up to n */
    for (idx_t k = cur_max + 1; k <= n; ++k) {
        if (k == 1) {
            seq_cache[1] = (seq_t)1;   /* a(1) = 1 */
            /* no prime factor, nothing to update */
        } else {
            /* collect distinct prime factors via spf */
            idx_t temp = k;
            uint32_t p = spf[temp];
            /* initialise with first distinct prime */
            uint32_t chosen_p = p;
            idx_t     chosen_last = last_occ[p];

            /* skip duplicates */
            while (temp % p == 0) temp /= p;

            /* iterate remaining factors */
            while (temp > 1) {
                p = spf[temp];
                while (temp % p == 0) temp /= p;

                if (last_occ[p] < chosen_last) {
                    chosen_last = last_occ[p];
                    chosen_p = p;
                }
            }

            seq_cache[k] = (seq_t)chosen_p;
            last_occ[chosen_p] = k;    /* record new appearance */
        }
    }

    if (n > cur_max) cur_max = n;
    return seq_cache[n];
}

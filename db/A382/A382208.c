/*
 * A382208  Numbers k for which pi(bigomega(k)) = omega(k).
 * https://oeis.org/A382208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382208: Numbers k for which π(Ω(k)) = ω(k).
   The function generates terms by incrementally testing each integer
   k and memoizing the results.  It works for any build of seq_t and
   idx_t (including arbitrary‑precision seq_t). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* -------------- static storage ------------------------------------ */

static idx_t curr_k = 0;   /* last integer examined                     */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;   /* memoized terms                       */
#else
static seq_t *cache = NULL;   /* dynamic array of seq_t elements       */
static size_t cap = 0;          /* current capacity of cache              */
static size_t cache_len = 0;    /* number of stored terms                 */
#endif

/* -------------- helpers -------------------------------------------- */
#ifndef __cplusplus
/* Ensure that cache has space for at least 'needed' elements.      */
static void ensure_cache_size(size_t needed)
{
    if (cap >= needed)        return;
    size_t new_cap = cap ? cap : 64;
    while (new_cap < needed)  new_cap <<= 1;

    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* In case of allocation failure we simply abort; that is acceptable
     * for the purposes of the sequence generator. */
    if (!new_ptr) abort();

    cache = new_ptr;
    cap   = new_cap;
}
#endif

/* -------------- sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1 – out of bounds behavior allowed */
        return (seq_t)0;

#ifdef __cplusplus
    /* C++ implementation: use std::vector for automatic construction. */
    while (cache.size() < n) {
        ++curr_k;
        bool good = false;

        if (curr_k == 1) {
            /* 1 has Ω(1)=0 and ω(1)=0. */
            good = true;
        } else {
            /* Factorize to obtain distinct primes and exponents. */
            size_t dcnt = 0;
            uint64_t primes[32], exps[32];
            factor_u64((uint64_t)curr_k, primes, exps, &dcnt);

            /* Count distinct primes  ω(k) = dcnt. */
            seq_t omega_distinct = (seq_t)dcnt;

            /* Count total prime factors with multiplicity Ω(k).          */
            idx_t omega_total = 0;
            for (size_t i = 0; i < dcnt; ++i)
                omega_total += (idx_t)exps[i];

            /* Compute π(Ω(k)) and compare. */
            seq_t pi_val = compute_pi((idx_t)omega_total);
            good = (pi_val == omega_distinct);
        }

        if (good)
            cache.push_back((seq_t)curr_k);
    }
    return cache[n - 1];
#else
    /* C implementation: use a raw array. */
    while (cache_len < n) {
        ++curr_k;
        bool good = false;

        if (curr_k == 1) {
            good = true;
        } else {
            size_t dcnt = 0;
            uint64_t primes[32], exps[32];
            factor_u64((uint64_t)curr_k, primes, exps, &dcnt);

            seq_t omega_distinct = (seq_t)dcnt;

            idx_t omega_total = 0;
            for (size_t i = 0; i < dcnt; ++i)
                omega_total += (idx_t)exps[i];

            seq_t pi_val = compute_pi((idx_t)omega_total);
            good = (pi_val == omega_distinct);
        }

        if (good) {
            ensure_cache_size(cache_len + 1);
            cache[cache_len++] = (seq_t)curr_k;
        }
    }
    return cache[n - 1];
#endif
}

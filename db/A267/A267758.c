/*
 * A267758  If a(n) is prime, then a(n) = a(n+1) - a(n-1): lexicographic first
 *          permutation of the nonnegative integers with this property.
 * https://oeis.org/A267758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267758: lexicographically first permutation of the
 * nonnegative integers such that if a(n) is prime, then
 * a(n+1) = a(n) + a(n-1).  a(0)=0, a(1)=1.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isPrimeGeneric.h"

/* --------------------   hash‑set for 64‑bit values -------------------- */
static size_t used_hash_cap   = 16;   /* always a power of two   */
static size_t used_hash_len   = 0;    /* how many values are stored */
static uint64_t *used_hash_val = NULL;
static bool    *used_hash_omp = NULL;  /* occupied flag */

static void used_hash_init(void)
{
    used_hash_val   = (uint64_t*)calloc(used_hash_cap, sizeof(uint64_t));
    used_hash_omp   = (bool*)calloc(used_hash_cap, sizeof(bool));
}

static void used_hash_resize(void)
{
    size_t old_cap = used_hash_cap;
    size_t new_cap = old_cap * 2;
    uint64_t *new_val = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    bool    *new_omp = (bool*)calloc(new_cap, sizeof(bool));

    /* re‑hash existing values */
    for (size_t i = 0; i < old_cap; ++i) {
        if (used_hash_omp[i]) {
            uint64_t v = used_hash_val[i];
            size_t h = (v * 11400714819323198485ULL) >> (64 - __builtin_ctzll(new_cap));
            while (new_omp[h]) h = (h + 1) & (new_cap - 1);
            new_val[h] = v;
            new_omp[h] = true;
        }
    }
    free(used_hash_val);
    free(used_hash_omp);
    used_hash_val = new_val;
    used_hash_omp = new_omp;
    used_hash_cap = new_cap;
}

/* insert a 64‑bit value */
static void used_hash_insert(uint64_t v)
{
    if (used_hash_len * 3 >= used_hash_cap * 2) used_hash_resize();

    size_t h = (v * 11400714819323198485ULL) >> (64 - __builtin_ctzll(used_hash_cap));
    while (used_hash_omp[h]) {
        if (used_hash_val[h] == v) return;   /* already present */
        h = (h + 1) & (used_hash_cap - 1);
    }
    used_hash_val[h] = v;
    used_hash_omp[h] = true;
    ++used_hash_len;
}

/* test if a 64‑bit value is present */
static bool used_hash_contains(uint64_t v)
{
    if (used_hash_len == 0) return false;
    size_t h = (v * 11400714819323198485ULL) >> (64 - __builtin_ctzll(used_hash_cap));
    while (used_hash_omp[h]) {
        if (used_hash_val[h] == v) return true;
        h = (h + 1) & (used_hash_cap - 1);
    }
    return false;
}

/* -----------------   used table for values that don't fit in 64‑bit  --------------------- */
static seq_t *used_big_vals = NULL;
static size_t used_big_cap  = 16;
static size_t used_big_len  = 0;

static void used_big_init(void)
{
    used_big_vals = (seq_t*)calloc(used_big_cap, sizeof(seq_t));
}

static void used_big_insert(seq_t v)
{
    if (used_big_len >= used_big_cap) {
        used_big_cap *= 2;
        used_big_vals = (seq_t*)realloc(used_big_vals,
                        used_big_cap * sizeof(seq_t));
    }
    used_big_vals[used_big_len++] = v;
}

static bool used_big_contains(seq_t v)
{
    for (size_t i = 0; i < used_big_len; ++i)
        if (used_big_vals[i] == v) return true;
    return false;
}

/* -----------------   public used check / insert   --------------------- */
static inline bool used_contains(seq_t v)
{
    if (v <= (seq_t)UINT64_MAX)
        return used_hash_contains((uint64_t)v);
    else
        return used_big_contains(v);
}

static inline void used_insert(seq_t v)
{
    if (v <= (seq_t)UINT64_MAX)
        used_hash_insert((uint64_t)v);
    else
        used_big_insert(v);
}

/* ----------------------   prime cache (delegated to utils)   ---------------------- */
/* The helper is provided by the build system. */

/* ----------------------------   main function   ------------------------- */
seq_t a(idx_t n)
{
    /* static variables for memoisation */
    static seq_t *seq  = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;   /* next free index */
    static idx_t  next_candidate = 0;   /* smallest integer that might be unused */

    /* one‑time initialisation */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        /* a(0)=0, a(1)=1 */
        seq[0] = 0;
        seq[1] = 1;
        seq_len = 2;

        used_hash_init();
        used_big_init();
        used_insert(seq[0]);
        used_insert(seq[1]);

        next_candidate = 2;
    }

    /* extend the sequence until we have computed a(n) */
    while (seq_len <= (size_t)n) {
        size_t i = seq_len;    /* index to fill */

        seq_t prev = seq[i-1];
        bool prev_prime = isPrimeGeneric(prev);

        /* ----------  case: previous element is prime – forced value ----------
         */
        if (prev_prime) {
            seq_t forced = prev + seq[i-2];

            /* make sure the forced number is not already used  */
            if (used_contains(forced)) {
                /* this situation never occurs for the true sequence,
                   but keep the code safe */
                /* search for next unused candidate that would not clash
                   with fixed future constraint */
                seq_t cand = next_candidate;
                for (;;) {
                    if (!used_contains(cand)) {
                        /* check future constraint for this candidate */
                        seq_t next_forced = cand + prev;
                        if (!used_contains(next_forced)) {
                            forced = cand;
                            break;
                        }
                    }
                    ++cand;
                }
            }

            /* store forced value */
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = forced;
            used_insert(forced);

            /* advance next_candidate */
            for (;; ++next_candidate)
                if (!used_contains(next_candidate)) break;
            continue;   /* next element will be processed in next loop */
        }

        /* ----------  non‑prime previous element – choose minimal candidate ----------
         */
        /* find the smallest unused integer */
        for (;; ++next_candidate)
            if (!used_contains(next_candidate)) break;
        seq_t candidate = next_candidate;

        for (;;) {
            /* candidate is not already used by construction
               but we must check the future constraint */
            if (isPrimeGeneric(candidate)) {
                seq_t future = candidate + prev;
                if (used_contains(future)) {
                    /* this candidate would force a duplicate later */
                    ++candidate;
                    continue;
                }
            }
            break;   /* candidate accepted */
        }

        /* store chosen candidate */
        if (seq_len >= seq_cap) {
            seq_cap *= 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = candidate;
        used_insert(candidate);

        /* advance next_candidate for next iteration */
        for (;; ++next_candidate)
            if (!used_contains(next_candidate)) break;
    }

    return seq[n];
}

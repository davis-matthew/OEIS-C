/*
 * A243582  Integers of the form 8k+7 (A004771) that cannot be written as sum
 *          of four distinct squares.
 * https://oeis.org/A243582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A243582: the n‑th integer of the form
   8k+7 that cannot be represented as a sum of four distinct squares.
   It uses an incremental search of candidates and caches the found
   terms so that subsequent calls are fast. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Test whether m can be written as a^2 + b^2 + c^2 + d^2 with
   a < b < c < d. */
static bool can_represent_as_distinct_four_squares(seq_t m)
{
    for (idx_t a = 0; ; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        if (a2 >= m) break;            /* a^2 alone already exceeds m */

        seq_t rem_a = m - a2;

        idx_t max_b = (idx_t)floor_sqrt(rem_a);
        for (idx_t b = a + 1; b <= max_b; ++b) {
            seq_t b2 = (seq_t)b * (seq_t)b;
            seq_t rem_ab = rem_a - b2;
            if (rem_ab <= 0) break;

            idx_t max_c = (idx_t)floor_sqrt(rem_ab);
            for (idx_t c = b + 1; c <= max_c; ++c) {
                seq_t c2 = (seq_t)c * (seq_t)c;
                seq_t rem_cd = rem_ab - c2;
                if (rem_cd <= 0) break;

                seq_t d_sq = rem_cd;
                idx_t d = (idx_t)floor_sqrt(d_sq);
                if (d > c && (seq_t)d * (seq_t)d == d_sq) {
                    return true;        /* representation found */
                }
            }
        }
    }
    return false;                       /* no representation */
}

/* -------- cache handling ---------------------------------------- */
static seq_t *cache = NULL;   /* array of found terms */
static size_t cache_len = 0;  /* current length of cache */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t next_k = 0;      /* next k to test for candidate 8k+7 */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *newp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (newp == NULL) abort();      /* allocation failed */
        cache = newp;
        cache_cap = new_cap;
    }
}

/* Append a value to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* -------- main routine ------------------------------------------ */
seq_t a(idx_t n)
{
    /* Compute terms until we have at least n values cached. */
    while ((size_t)n > cache_len) {
        seq_t candidate = (seq_t)8 * next_k + 7;
        if (!can_represent_as_distinct_four_squares(candidate)) {
            cache_append(candidate);
        }
        ++next_k;
    }
    return cache[n - 1];  /* a(1) is stored at cache[0] */
}

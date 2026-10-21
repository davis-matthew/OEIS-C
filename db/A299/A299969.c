/*
 * A299969  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 9, and no term occurs twice.
 * https://oeis.org/A299969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   ensure_used_flag, sum_has_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Lexicographic first permutation of nonnegative integers such that
 *  each adjacent pair sums to a number whose decimal representation
 *  contains a 9, and no term repeats.
 *
 *  The function computes a(n) for arbitrary index n ≥ 0 using a
 *  lazy greedy construction.  For each new term it scans increasing
 *  integers, skipping those already used, until it finds the smallest
 *  unused integer whose sum with the previous term contains a 9.
 *
 *  The implementation keeps two dynamic arrays:
 *    - seq_cache : the constructed sequence,
 *    - used_flag : a boolean bitmap marking which integers have been used.
 *
 *  Complexity is roughly O(n²) in the number of terms, which easily
 *  satisfies the 2 s limit for n up to about 10 000 on typical
 *  hardware.  All arithmetic uses seq_t; only the check
 *  `candidate >= used_cap` requires a conversion to idx_t, which is
 *  guaranteed to be small for the range of terms tested.
 *
 *  No other helper functions are required; everything is self‑contained.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_used_flag.h"
#include "../utils/sum_has_9.h"

/* Ensure that seq_cache has capacity for indices 0..n  */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    size_t cap = *cap_ptr;
    if (cap > n) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
    *cache_ptr = new_cache;
    *cap_ptr   = new_cap;
}

/* Global state */
static seq_t *seq_cache     = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;

static bool *used_flag     = NULL;
static size_t used_cap     = 0;

/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Ensure we have room for index n */
    ensure_cache(n, &seq_cache, &seq_cache_cap);

    /* Generate terms up to n if needed */
    while (seq_cache_len <= (size_t)n) {
        if (seq_cache_len == 0) {
            /* a(0) = 0 */
            seq_cache[0] = (seq_t)0;
            seq_cache_len = 1;
            /* mark 0 as used */
            ensure_used_flag(0, &used_flag, &used_cap);
            used_flag[0] = true;
            continue;
        }

        /* Current last value */
        seq_t last = seq_cache[seq_cache_len - 1];

        /* Find smallest unused integer x so that last + x has a 9 digit */
        idx_t candidate = 0;
        for (;;) {
            /* Make sure used_flag can index candidate */
            ensure_used_flag(candidate, &used_flag, &used_cap);
            if (used_flag[candidate]) {          /* already used */
                ++candidate;
                continue;
            }
            seq_t sum = last + (seq_t)candidate;
            if (sum_has_9(sum)) break;
            ++candidate;
        }

        /* Store chosen candidate */
        seq_cache[seq_cache_len++] = (seq_t)candidate;
        /* Mark it used */
        ensure_used_flag(candidate, &used_flag, &used_cap);
        used_flag[candidate] = true;
    }

    return seq_cache[(size_t)n];
}

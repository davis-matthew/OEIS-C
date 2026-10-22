/*
 * A318907  Numbers m such that A006666(m)/A006667(m) is an integer.
 * https://oeis.org/A318907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A318907  –  Numbers m such that A006666(m)/A006667(m) is an integer.
 *  Formula: a(n) = the n‑th integer m ≥ 2 for which
 *   A006666(m)/A006667(m) is integral (i.e. halving_steps(m) % tripling_steps(m) == 0, tripling_steps(m) > 0).
 *
 *  The implementation keeps a memoised cache of found terms.  For each candidate m
 *  it runs the standard Collatz sequence until 1 while counting halving and tripling
 *  steps using only the arithmetic operators available for seq_t.  If the ratio
 *  is integral the number is stored in the cache.  Calls to a(n) retrieve the
 *  n‑th element from the cache, extending it on demand.
 *
 *  This code satisfies the constraints of C11 and C++17, uses only the allowed
 *  operators on seq_t, and does not rely on non‑portable integer promotions.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Dynamic array helpers ----------------------------------------------------- */
static void *seq_alloc(size_t size)
{
    return malloc(size);
}
static void seq_free(void *p)
{
    free(p);
}
static void seq_realloc(void **p, size_t old_cap, size_t new_cap, size_t elem_size)
{
    void *newp = realloc(*p, new_cap * elem_size);
    *p = newp;
}
static void ensure_capacity(size_t needed, size_t *cap, void **store, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 8 : (*cap << 1));
        while (new_cap < needed) new_cap <<= 1;
        seq_realloc(store, *cap, new_cap, elem_size);
        *cap = new_cap;
    }
}

/* Global state -------------------------------------------------------------- */
static seq_t *cache = NULL;          /* cached sequence values          */
static size_t cache_len = 0;         /* number of cached terms          */
static size_t cache_cap = 0;         /* capacity of the cache array     */
static idx_t next_candidate = 2;     /* next integer to test            */

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1: a(1) is the first term.  n must be >= 1 for a
       valid query.  For robustness, return 0 for an index 0. */
    if (n == 0) return 0;

    /* Extend the cache until it contains at least n terms.  Each new
       candidate m is tested for the integral ratio property. */
    while (cache_len < (size_t)n) {
        seq_t m = (seq_t)next_candidate;
        seq_t halving = 0;
        seq_t trip    = 0;

        seq_t x = m;
        while (x != (seq_t)1) {
            if ((x & (seq_t)1) == (seq_t)0) {   /* halving step */
                halving++;
                x = x >> 1;
            } else {                            /* tripling step (3x+1) */
                trip++;
                x = x * 3 + 1;
            }
        }

        if (trip != (seq_t)0 && (halving % trip) == (seq_t)0) {
            /* Store the term in the cache. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = m;
        }

        next_candidate++;
    }

    return cache[n - 1];
}

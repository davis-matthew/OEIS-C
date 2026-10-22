/*
 * A338844  No prime digits are present in a(n) * a(n+1).
 * https://oeis.org/A338844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1081, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   no_prime_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: A338844 – lexicographically earliest permutation of
   the nonnegative integers such that no prime digit (2,3,5,7) occurs
   in the decimal representation of a(n) * a(n+1). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/no_prime_digits.h"

/* Global state for memoisation */
static seq_t *seq_cache = NULL;   /* already generated terms          */
static size_t cache_cap   = 0;    /* allocated size of seq_cache      */
static bool   *used       = NULL;/* used[v] == true  ⇔  v already chosen */
static size_t used_cap    = 0;    /* allocated size of used[]         */
static idx_t  max_generated = 0;  /* number of terms that exist      */

/* Ensure seq_cache can hold at least n elements. */
static void ensure_cache(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Ensure used can address at least n distinct candidate values. */
static void ensure_used(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < n) new_cap <<= 1;
    bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
    /* initialise the newly allocated portion to false */
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Return the n-th term of A338844 (n ≥ 1).  The function is fully memoised
   and may be called in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1 – this value is unused   */

    /* Generate terms up to the requested index if necessary. */
    while ((idx_t)max_generated < n) {
        seq_t prev = (max_generated == 0) ? 0 : seq_cache[max_generated - 1];
        idx_t cand = 0;
        for (;;) {
            /* grow the used array if the candidate exceeds current size. */
            if ((size_t)cand >= used_cap) ensure_used((size_t)cand + 1);

            if (used[(size_t)cand]) { ++cand; continue; }

            /* check the product condition, except for the very first term */
            if (max_generated > 0) {
                seq_t prod = prev * (seq_t)cand;
                if (!no_prime_digits(prod)) { ++cand; continue; }
            }

            /* candidate accepted */
            used[(size_t)cand] = true;
            if ((size_t)max_generated >= cache_cap) ensure_cache(max_generated + 1);
            seq_cache[max_generated] = (seq_t)cand;
            ++max_generated;
            break;
        }
    }
    return seq_cache[n - 1];
}

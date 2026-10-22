/*
 * A366197  Lexicographically earliest permutation of the nonnegative integers
 *          such that the absolute difference between the digitsum of a(n) and
 *          the digitsum of a(n+2) = 1.
 * https://oeis.org/A366197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366197:
   Lexicographically earliest permutation of the nonnegative integers
   such that |digit_sum(a(n)) - digit_sum(a(n+2))| = 1.
   The implementation builds the sequence lazily, memoising already
   computed terms.  Each term is chosen greedily: at position n
   (n ≥ 0) pick the smallest unused integer that satisfies the
   constraint with a(n‑2); for n = 0 or 1 no restriction applies.  */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <string.h>   /* memset */
#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/digit_sum.h"

/* Grow a boolean array to at least 'need' elements, clearing new slots. */
static void ensure_used_capacity(size_t *cap, bool **arr, size_t need)
{
    if (*cap >= need)
        return;
    size_t new_cap = (*cap > 0 ? *cap : 1);
    while (new_cap < need)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(*arr, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    memset(tmp + *cap, 0, (new_cap - *cap) * sizeof(bool));
    *arr   = tmp;
    *cap   = new_cap;
}

/* The required function */
seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t last_filled = 0;   /* number of terms currently stored */

    /* Used flag array : used[i] == true <=> i already appears in a */
    static bool *used  = NULL;
    static size_t used_cap = 0;

    /* Keep track of the smallest unused integer (recomputed each time) */
    static size_t next_unused = 0;

    /* Resize cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap > 0 ? cache_cap : 1);
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Build terms up to n */
    for (size_t idx = last_filled; idx <= (size_t)n; ++idx) {
        seq_t chosen;

        /* Ensure we know smallest unused integer */
        if (next_unused < used_cap) {
            while (next_unused < used_cap && used[next_unused])
                ++next_unused;
        }
        else {
            /* used array may be incomplete – grow it */
            ensure_used_capacity(&used_cap, &used, next_unused + 1);
            while (next_unused < used_cap && used[next_unused])
                ++next_unused;
        }

        if (idx < 2) {                     /* no restriction for n = 0,1 */
            chosen = (seq_t)next_unused;
        } else {
            /* Search for the minimal unused integer satisfying |DS(prev)-DS(x)|==1 */
            size_t m = next_unused;
            while (true) {
                ensure_used_capacity(&used_cap, &used, m + 1);
                if (!used[m]) {
                    seq_t ds_prev = digit_sum(cache[idx - 2]);
                    seq_t ds_m   = digit_sum((seq_t)m);
                    seq_t diff   = (ds_prev > ds_m) ? (ds_prev - ds_m) : (ds_m - ds_prev);
                    if (diff == 1) {
                        chosen = (seq_t)m;
                        break;
                    }
                }
                ++m;
            }
        }

        /* Record the term and mark it used */
        cache[idx] = chosen;
        ensure_used_capacity(&used_cap, &used, (size_t)chosen + 1);
        used[(size_t)chosen] = true;

        /* Recompute smallest unused integer for the next position */
        next_unused = 0;
        while (next_unused < used_cap && used[next_unused])
            ++next_unused;
    }

    last_filled = (size_t)n + 1;
    return cache[(size_t)n];
}

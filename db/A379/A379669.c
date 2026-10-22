/*
 * A379669  Number of finite multisets of positive integers > 1 with sum +
 *          product = n.
 * https://oeis.org/A379669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of finite multisets of integers > 1 whose sum plus
 *            product equals n. The implementation enumerates all such
 *            multisets using a back‑tracking search up to the queried n,
 *            memoising results in a static array. The array is rebuilt
 *            whenever a larger index is requested. This runs in O(n ·...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Forward declaration of the recursive enumeration routine. */
static void dfs_rec(idx_t limit, seq_t *cnt, idx_t min_val, idx_t sum, seq_t prod);

/* Memoised cache of computed values and the largest index that has been computed. */
static seq_t *cached_counts = NULL;
static idx_t cached_max    = 0;

/* Recompute the cached_counts array for all indices up to and including N. */
static void recompute_counts_upto(idx_t N)
{
    /* Allocate a new zero‑initialised array of size N+1. */
    size_t sz = (size_t)N + 1;
    seq_t *new_counts = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_counts)
        return; /* Allocation failure – leave cache empty. */

    /* The empty multiset has sum = 0, product = 1, so n = 1. */
    new_counts[1] = (seq_t)1;

    /* Enumerate all non‑empty multisets of integers > 1. */
    dfs_rec(N, new_counts, (idx_t)2, (idx_t)0, (seq_t)1);

    /* Replace the old cached array (freeing it first). */
    free(cached_counts);

    cached_counts = new_counts;
    cached_max    = N;
}

/* Recursive enumeration of multisets.
   Parameters:
     limit   – the maximum value of n that we are interested in
     cnt     – array accumulating the counts
     min_val – smallest part allowed in the current multiset (enforces non‑decreasing order)
     sum     – current sum of the partial multiset
     prod    – current product of the partial multiset */
static void dfs_rec(idx_t limit, seq_t *cnt, idx_t min_val, idx_t sum, seq_t prod)
{
    idx_t val = min_val;
    for (;; ++val) {
        /* Avoid overflow when computing prod * val. */
        if (prod != 0 && prod > (seq_t)limit / (seq_t)val)
            break;
        seq_t new_prod = prod * (seq_t)val;

        /* New sum.  If it would alone exceed the limit, stop the loop. */
        idx_t new_sum = sum + val;
        if (new_sum > limit)
            break;

        /* n = sum + product.  If this exceeds the limit, further larger vals
           will also exceed it. */
        seq_t n_val = (seq_t)new_sum + new_prod;
        if (n_val > (seq_t)limit)
            break;

        /* Count this multiset. */
        cnt[(size_t)n_val] += (seq_t)1;

        /* Recurse to add more elements, preserving non‑decreasing order. */
        dfs_rec(limit, cnt, val, new_sum, new_prod);
    }
}

/* Public API function required by the build system. */
seq_t a(idx_t n)
{
    /* Compute values up to n if we have not done so yet. */
    if (n > cached_max || cached_counts == NULL)
        recompute_counts_upto(n);

    /* The cached array is guaranteed to be large enough. */
    return cached_counts[(size_t)n];
}

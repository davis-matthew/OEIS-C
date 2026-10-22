/*
 * A360954  Number of finite sets of positive integers whose right half
 *          (exclusive) sums to n.
 * https://oeis.org/A360954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{w>=1} Sum_{h=w+1..floor((n-C(w,2))/w)} C(h,w+1) *
 *            A072233(n - w*h - C(w,2), w-1) with A072233(n,k) = number of
 *            partitions of n into at most k parts. a(0) = 1. The
 *            implementation memoises a partition table p_k(n) = number of
 *            partitions of n into at most k parts
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 177, u64 n <= 616,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binom.h"

/* Partition table: p_k(n) stored row‑wise as [n][k].  The table is
   rebuilt (without zeroing garbage) whenever a larger n or k is
   requested.  The table is lazily built on demand and reused for
   subsequent calls. */
static idx_t part_Nmax = 0;          /* maximum n stored (inclusive)          */
static idx_t part_Kmax = 0;          /* maximum k stored (inclusive)          */
static seq_t *part_table = NULL;     /* linear array [n][k]                   */

/* Ensure the partition table covers the desired ranges.  The table
   is allocated with calloc to initialise all entries to 0.  After
   allocation the base case row (n==0) is set to 1 for all k. */
static void ensure_partition_table(idx_t Nmax, idx_t Kmax)
{
    if (Nmax <= part_Nmax && Kmax <= part_Kmax)
        return;                       /* already sufficient */

    /* free old table */
    if (part_table) {
        free(part_table);
        part_table = NULL;
    }

    part_Nmax = Nmax;
    part_Kmax = Kmax;

    size_t total = (size_t)(part_Nmax + 1) * (part_Kmax + 1);
    part_table = (seq_t *)calloc(total, sizeof(seq_t));
    if (!part_table)
        abort();                      /* out of memory */

    /* base case: p_0(n)=1, p_k(0)=1 for all k>=0. */
    for (idx_t k = 0; k <= part_Kmax; ++k)
        part_table[(size_t)k] = 1;     /* n==0 row, index = 0*(Kmax+1)+k */

    /* recurrence: p_n(k) = p_n(k-1) + p_{n-k}(k) if n>=k else p_n(k-1) */
    for (idx_t k = 1; k <= part_Kmax; ++k) {
        for (idx_t n = 1; n <= part_Nmax; ++n) {
            size_t idx = (size_t)n * (part_Kmax + 1) + (size_t)k;
            seq_t val = part_table[(size_t)n * (part_Kmax + 1) + (size_t)(k - 1)];
            if (n >= k)
                val += part_table[(n - k) * (part_Kmax + 1) + (size_t)k];
            part_table[idx] = val;
        }
    }
}

/* Helper to fetch a partition value from the memoised table. */
static inline seq_t partitions_at_most_k(idx_t n, idx_t k)
{
    return part_table[(size_t)n * (part_Kmax + 1) + (size_t)k];
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute w_max from inequality w*(w-1)/2 <= n  (floored) */
    idx_t w_max = (idx_t)((1.0 + sqrt((double)(1 + 8 * (double)n))) / 2.0);

    /* Ensure the partition table can answer all needed queries. */
    ensure_partition_table(n, w_max);

    seq_t result = 0;

    for (idx_t w = 1; w <= w_max; ++w) {
        seq_t bin_w2 = binom(w, 2);                 /* C(w,2) */

        /* upper bound for h */
        idx_t h_max = 0;
        if (n >= (idx_t)bin_w2)
            h_max = (n - (idx_t)bin_w2) / w;
        if (h_max < w + 1)            /* no admissible h */
            continue;

        idx_t h_min = w + 1;
        for (idx_t h = h_min; h <= h_max; ++h) {
            seq_t bin_hex = binom(h, w + 1);        /* C(h,w+1) */
            idx_t rem = n - w * h - (idx_t)bin_w2;  /* remaining sum >= 0 */
            seq_t part_val = partitions_at_most_k(rem, w - 1);
            result += bin_hex * part_val;
        }
    }
    return result;
}

/*
 * A246187  Triangle read by rows: T(n,k) is the number of weighted lattice
 *          paths in F[n] having length k (length = number of steps). The
 *          members of F[n] are paths of weight n that start at (0,0), do not
 *          go below the horizontal axis, and whose steps are of the following
 *          four kinds: an (1,0)-step with weight...
 * https://oeis.org/A246187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 129, u32 n <= 424, u64 n <= 1412,
 *            u128 n <= 5018, big any n
 * Helpers:   free_seq_arr, weight_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T[w][l] where
 *   w  = weight_needed(n)      (the row containing index n)
 *   l  = n - w*(w+1)/2         (the column within that row)
 *   T[w][l] = number of weighted lattice paths of total weight w
 *             and length l, counted in F[w] as defined in OEIS A246187.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/free_seq_arr.h"
#include "../utils/weight_needed.h"

/* ---------- memory helpers for seq_t (C/C++ compatible) ---------- */
static seq_t *allocate_seq_arr(size_t count)
{
#ifdef __cplusplus
    return new seq_t[count]();          /* value‑initialize for C++ */
#else
    return (seq_t *)calloc(count, sizeof(seq_t));
#endif
}

/* ---------- build all rows up to max_w and store in a triangular array T ---------- */
static void build_triangle(idx_t max_w, seq_t **T_out)
{
    const idx_t W = max_w;
    const size_t stride = (size_t)W + 1;                       /* stride for DP indexing */
    const size_t dp_size = stride * stride * stride;           /* (W+1)^3 */

    seq_t *dp = allocate_seq_arr(dp_size);
    if (!dp) abort();                                         /* allocation failure */

    /* initialise only the empty path */
    dp[0] = 1;

    /* DP: iterate through all attainable states */
    for (idx_t w = 0; w <= W; ++w) {
        for (idx_t l = 0; l <= w; ++l) {
            for (idx_t h = 0; h <= w; ++h) {
                seq_t val = dp[(size_t)((w * stride + l) * stride + h)];
                if (!val) continue;

                /* (1,0)-step of weight 1 (horizontal) */
                if (w + 1 <= W)
                    dp[(size_t)((w + 1) * stride + (l + 1)) * stride + h] += val;

                /* (1,-1)-step of weight 1 (down) */
                if (w + 1 <= W && h > 0)
                    dp[(size_t)((w + 1) * stride + (l + 1)) * stride + (h - 1)] += val;

                /* (1,0)-step of weight 2 (horizontal) */
                if (w + 2 <= W)
                    dp[(size_t)((w + 2) * stride + (l + 1)) * stride + h] += val;

                /* (1,1)-step of weight 2 (up) */
                if (w + 2 <= W)
                    dp[(size_t)((w + 2) * stride + (l + 1)) * stride + (h + 1)] += val;
            }
        }
    }

    /* allocate triangular array T with entries T[w][l] (l = 0..w) */
    const size_t total_terms = stride * (stride + 1) / 2;      /* (W+1)(W+2)/2 */
    seq_t *T = allocate_seq_arr(total_terms);
    if (!T) {
        free_seq_arr(dp);
        abort();
    }

    for (idx_t w = 0; w <= W; ++w) {
        size_t offset = (size_t)w * (w + 1) / 2;
        for (idx_t l = 0; l <= w; ++l) {
            seq_t sum = 0;
            for (idx_t h = 0; h <= w; ++h)
                sum += dp[(size_t)((w * stride + l) * stride + h)];
            T[offset + l] = sum;
        }
    }

    free_seq_arr(dp);
    *T_out = T;
}

/* ---------- static cache for the computed triangle ---------- */
static seq_t *cached_T = NULL;
static idx_t cached_wmax = 0;
static bool cache_valid = false;

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* find the row weight that contains this index */
    idx_t needed_w = weight_needed(n);

    /* rebuild cache if necessary (only grows) */
    if (!cache_valid || needed_w > cached_wmax) {
        if (cached_T) free_seq_arr(cached_T);
        build_triangle(needed_w, &cached_T);
        cached_wmax = needed_w;
        cache_valid = true;
    }

    /* map global n to triangular coordinates (w, l) */
    size_t row_start = (size_t)needed_w * (needed_w + 1) / 2;
    idx_t l = (idx_t)(n - row_start);          /* 0 <= l <= needed_w */

    return cached_T[row_start + l];
}

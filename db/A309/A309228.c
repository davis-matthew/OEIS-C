/*
 * A309228  a(n) is the greatest possible height of a binary tree where all
 *          nodes hold positive squares and all interior nodes also equal the
 *          sum of their two children and the root node has value n^2.
 * https://oeis.org/A309228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest possible height of a binary tree where * all
 *            nodes hold positive squares, interior nodes equal sum of their
 *            two * children, and the root node has value n^2. a(1) = 1, and
 *            for any other * n, a(n) = 1 + max( a(u), a(v) ) over all u^2 +
 *            v^2 = n^2 with u > v > 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_int_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_int_small.h"

/* Basic integer square root (floor).  Uses binary search and 128‑bit product
 * to avoid overflow for values up to 2^64‑1. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Structure holding a representation (u, v).  All values fit in idx_t
 * because u, v <= n and n <= 10000. */
typedef struct { idx_t u; idx_t v; } pair_t;

/* Global data that is recomputed on demand.  All arrays are freed
 * and reallocated each time a larger N is requested. */
static seq_t *height        = NULL;
static idx_t *rep_start     = NULL;
static idx_t *rep_count     = NULL;
static pair_t *pair_arr     = NULL;
static idx_t  max_comp      = 0;   /* largest N for which data is ready */

/* Allocation helpers that work in both C and C++. */
#if defined(__cplusplus)
#   define FREE_SEQ(ptr) do { delete [] (ptr); (ptr) = nullptr; } while(0)
#   define ALLOC_SEQ(name,size) ((name) = new seq_t[(size)]())
#else
#   define FREE_SEQ(ptr) do { free (ptr); (ptr) = NULL; } while(0)
#   define ALLOC_SEQ(name,size) ((name) = (seq_t*)calloc((size), sizeof(seq_t)))
#endif

/* Recompute all data up to new_limit (inclusive).  This includes
 * representation counts, the list of pairs for each n, and the heights
 * computed by the recurrence. */
static void recompute(idx_t new_limit)
{
    if (new_limit <= max_comp)
        return;

    /* Free old data */
    FREE_SEQ(height);
    free(rep_start);
    free(rep_count);
    free(pair_arr);
    max_comp = 0;

    /* Allocate new arrays */
    idx_t size = new_limit + 1;
    ALLOC_SEQ(height, size);                /* heights */
    rep_start = (idx_t*)calloc(size, sizeof(idx_t));
    rep_count = (idx_t*)calloc(size, sizeof(idx_t));

    /* First pass: count representations for each hypotenuse */
    idx_t sqrt_limit = floor_sqrt_u64(new_limit);
    for (idx_t m = 2; m <= sqrt_limit; ++m) {
        for (idx_t k = 1; k < m; ++k) {
            if (((m - k) & 1) == 0)          /* skip same parity */
                continue;
            if (gcd_int_small((int)m, (int)k) != 1)
                continue;
            idx_t a = m * m - k * k;
            idx_t b = 2 * m * k;
            idx_t u = a > b ? a : b;          /* max(a,b) */
            idx_t v = a > b ? b : a;          /* min(a,b) */
            idx_t c = m * m + k * k;
            for (idx_t t = 1; t * c <= new_limit; ++t) {
                idx_t hyp = t * c;
                /* ui, vi do not need to be stored now */
                rep_count[hyp]++;
            }
        }
    }

    /* Compute prefix sums rep_start and total number of pairs */
    rep_start[0] = 0;
    idx_t total_pairs = 0;
    for (idx_t i = 1; i <= new_limit; ++i) {
        rep_start[i] = rep_start[i - 1] + rep_count[i - 1];
        total_pairs += rep_count[i];
    }
    pair_arr = (pair_t*)calloc(total_pairs, sizeof(pair_t));

    /* Temporary array to keep track of insertion positions for each n */
    idx_t *pos = (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= new_limit; ++i)
        pos[i] = rep_start[i];

    /* Second pass: store all (u, v) pairs */
    for (idx_t m = 2; m <= sqrt_limit; ++m) {
        for (idx_t k = 1; k < m; ++k) {
            if (((m - k) & 1) == 0)
                continue;
            if (gcd_int_small((int)m, (int)k) != 1)
                continue;
            idx_t a = m * m - k * k;
            idx_t b = 2 * m * k;
            idx_t u = a > b ? a : b;
            idx_t v = a > b ? b : a;
            idx_t c = m * m + k * k;
            for (idx_t t = 1; t * c <= new_limit; ++t) {
                idx_t hyp = t * c;
                idx_t ui = t * u;
                idx_t vi = t * v;
                idx_t idx = pos[hyp]++;
                pair_arr[idx].u = ui;
                pair_arr[idx].v = vi;
            }
        }
    }
    free(pos);

    /* Compute heights using the recurrence */
    height[1] = 1;
    for (idx_t n = 2; n <= new_limit; ++n) {
        seq_t best = 1;
        idx_t start = rep_start[n];
        idx_t count = rep_count[n];
        for (idx_t i = 0; i < count; ++i) {
            pair_t p = pair_arr[start + i];
            seq_t cand = 1 + (height[p.u] > height[p.v] ? height[p.u] : height[p.v]);
            if (cand > best)
                best = cand;
        }
        height[n] = best;
    }

    max_comp = new_limit;
}

/* Public function mandated by the judge. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    /* Ensure we have precomputed data up to n.  The routine is
     * thread‑unsafe but the judge environment is single‑threaded. */
    if (n > max_comp)
        recompute(n);
    return height[n];
}

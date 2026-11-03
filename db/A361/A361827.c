/*
 * A361827  For any number k >= 0, let T_k be the triangle whose base
 *          corresponds to the ternary expansion of k (without leading zeros)
 *          and other values, say t above u and v, satisfy t = (-u-v) mod 3;
 *          this sequence lists the numbers k such that the configurations of
 *          0's, 1's and 2's in T_k are the same up to...
 * https://oeis.org/A361827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*(n+1)/2
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A361827.  For each k we construct the
   triangle T_k and test whether every orbit of the 120° rotation
   contains one of each symbol 0,1,2; i.e., the configuration is
   the same up to a permutation of the symbols.  The sequence is
   generated incrementally and cached for random accesses. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true iff the triangle T_k has the required symmetry. */
static bool candidate_is_valid(seq_t k)
{
    /* Extract base‑3 digits (least significant first). */
    uint8_t digits[4096];
    size_t len = 0;
    seq_t t = k;
    if (t == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t != (seq_t)0) {
            digits[len++] = (uint8_t)(t % 3);
            t /= 3;
        }
    }

    if (len <= 1) return false;          /* need at least two rows */
    if (len % 3 == 1) return false;      /* would have a fixed point */

    size_t total = len * (len + 1) / 2;

    /* Buffer for the triangle values.  Grow as necessary. */
    static seq_t *tri = NULL;
    static size_t tri_cap = 0;
    if (total > tri_cap) {
        seq_t *new_tri = mem_resize(seq_t, tri, total);
        if (!new_tri) return false;          /* allocation failure */
        tri = new_tri;
        tri_cap = total;
    }

    /* Bottom row (row len‑1). */
    size_t start_base = (len - 1) * len / 2;
    for (size_t c = 0; c < len; ++c) {
        tri[start_base + c] = (seq_t)digits[len - 1 - c];
    }

    /* Build the remaining rows upward. */
    for (size_t r = len - 2; ; --r) {
        size_t start_r = r * (r + 1) / 2;
        size_t start_next = (r + 1) * (r + 2) / 2;
        for (size_t c = 0; c <= r; ++c) {
            seq_t u = tri[start_next + c];
            seq_t v = tri[start_next + c + 1];
            seq_t uv = u + v;
            tri[start_r + c] = (3 - (uv % 3)) % 3;
        }
        if (r == 0) break;
    }

    /* Visitation array for orbit traversal.  Grow as necessary. */
    static bool *vis = NULL;
    static size_t vis_cap = 0;
    if (total > vis_cap) {
        bool *new_vis = (bool *)realloc(vis, total * sizeof(bool));
        if (!new_vis) return false;
        vis = new_vis;
        vis_cap = total;
    }
    memset(vis, 0, total * sizeof(bool));

    /* Check each orbit (size 3). */
    for (size_t r = 0; r < len; ++r) {
        size_t start_r = r * (r + 1) / 2;
        for (size_t c = 0; c <= r; ++c) {
            size_t idx = start_r + c;
            if (vis[idx]) continue;

            int mask = 0;
            size_t rr = r, cc = c;
            for (int step = 0; step < 3; ++step) {
                size_t i = rr * (rr + 1) / 2 + cc;
                vis[i] = true;
                seq_t val = tri[i];
                mask |= 1 << (int)val;

                /* Rotate point by 120°. */
                size_t rp = (len - 1) - cc;
                size_t cp = rr - cc;
                rr = rp;
                cc = cp;
            }
            if (mask != 7) return false;
        }
    }
    return true;
}

/* Static cache for generated sequence values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n=0; harmless */

    while (cache_len < (size_t)n) {
        seq_t k = next_candidate;
        while (!candidate_is_valid(k)) ++k;   /* find next valid term */

        /* Append to cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (!new_cache) return (seq_t)0;  /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;
        next_candidate = k + (seq_t)1;
    }
    return cache[n - 1];
}

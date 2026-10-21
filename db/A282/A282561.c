/*
 * A282561  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that both 2*x - y and 4*z^2 + 724*z*w +
 *          w^2 are squares.
 * https://oeis.org/A282561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y,z,w) >=0 : x² + y² + z² + w² = n, * 2x - y is a
 *            square and 4z² + 724zw + w² is a square }
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple O(max_n²) convolution.  For n up to
 * 10 000 the loops run in milliseconds.  The routine keeps its
 * results cached so that repeated calls are inexpensive.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *a_cache   = NULL;
static seq_t *f1_cache  = NULL;
static seq_t *f2_cache  = NULL;
static idx_t  cache_max = 0;

static void recompute(idx_t max_n)
{
    /* free old cache */
    if (a_cache) {
        free(a_cache);
        free(f1_cache);
        free(f2_cache);
        a_cache = f1_cache = f2_cache = NULL;
    }

    /* allocate new arrays */
    size_t sz = (size_t)(max_n + 1);
    a_cache  = (seq_t*)calloc(sz, sizeof(seq_t));
    f1_cache = (seq_t*)calloc(sz, sizeof(seq_t));
    f2_cache = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!a_cache || !f1_cache || !f2_cache)
        return;   /* allocation failure, leave arrays NULL */

    /* pre‑compute f1 : pairs (x,y) with 2x – y a square */
    for (idx_t x=0; (idx_t)x*x <= max_n; ++x) {
        idx_t xsq = x*x;
        for (idx_t y=0; xsq + y*y <= max_n; ++y) {
            idx_t sum = xsq + y*y;
            seq_t diff = (seq_t)2 * x;
            if (diff < y) continue;
            diff -= y;
            seq_t sq = floor_sqrt(diff);
            if (sq*sq == diff) f1_cache[sum] += 1;
        }
    }

    /* pre‑compute f2 : pairs (z,w) with 4z² + 724zw + w² a square */
    for (idx_t z=0; (idx_t)z*z <= max_n; ++z) {
        idx_t zsqs = z*z;
        for (idx_t w=0; zsqs + w*w <= max_n; ++w) {
            idx_t sum = zsqs + w*w;
            seq_t val = (seq_t)4*zsqs + (seq_t)724*z*w + (seq_t)w*w;
            seq_t sq = floor_sqrt(val);
            if (sq*sq == val) f2_cache[sum] += 1;
        }
    }

    /* convolution f1 * f2 → a_cache */
    for (idx_t i=0; i<=max_n; ++i) {
        if (!f1_cache[i]) continue;
        for (idx_t j=0; j<=max_n-i; ++j) {
            if (!f2_cache[j]) continue;
            a_cache[i+j] += f1_cache[i]*f2_cache[j];
        }
    }
}

seq_t a(idx_t n)
{
    if (n > cache_max || a_cache == NULL) {
        recompute(n);
        cache_max = n;
        if (!a_cache) return 0;   /* allocation failed */
    }
    return a_cache[n];
}

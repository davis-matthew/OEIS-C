/*
 * A305260  A linear mapping a(n) = x + y*n of pairs of nonnegative integers
 *          (x,y), where the pairs are enumerated first by radial coordinate r
 *          and in case of ties, by polar angle 0 <= phi <= Pi/2 in a polar
 *          coordinate system.
 * https://oeis.org/A305260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = x + y*n  where (x,y) is the n-th pair of non‑negative
 * integers sorted first by increasing squared distance d = x² + y²,
 * and for equal d sorted by increasing y (equivalently by polar angle
 * in [0,π/2]).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) */
static inline idx_t _isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Append all (x,y) with x² + y² == d to the cache, sorted by increasing y. */
static void _add_pairs_for_d(idx_t d,
                             idx_t **xs, idx_t **ys,
                             size_t *cap, size_t *len)
{
    idx_t max_x = _isqrt_u64(d);
    /* Temporary storage – the number of representations of a number
       as a sum of two squares is always < 128 for d < 10¹⁰, enough here. */
    idx_t tmp_x[128];
    idx_t tmp_y[128];
    size_t cnt = 0;

    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t x2 = x * x;
        if (x2 > d) break;
        idx_t y2 = d - x2;
        idx_t y = _isqrt_u64(y2);
        if (y * y == y2) {
            tmp_x[cnt] = x;
            tmp_y[cnt] = y;
            ++cnt;
        }
    }
    /* Sort by y ascending – cnt is tiny, so insertion sort is fine. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (tmp_y[i] > tmp_y[j]) {
                idx_t tx = tmp_x[i];
                idx_t ty = tmp_y[i];
                tmp_x[i] = tmp_x[j];
                tmp_y[i] = tmp_y[j];
                tmp_x[j] = tx;
                tmp_y[j] = ty;
            }
        }
    }

    /* Append to cache, expanding if necessary. */
    for (size_t i = 0; i < cnt; ++i) {
        if (*len + 1 > *cap) {
            size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
            idx_t *newxs = (idx_t *)realloc(*xs, newcap * sizeof(idx_t));
            idx_t *newys = (idx_t *)realloc(*ys, newcap * sizeof(idx_t));
            *xs = newxs;
            *ys = newys;
            *cap = newcap;
        }
        (*xs)[*len] = tmp_x[i];
        (*ys)[*len] = tmp_y[i];
        ++(*len);
    }
}

seq_t a(idx_t n) {
    /* Static cache of pairs (x,y).  The cache holds pairs in the order
       established by the distance rule.  It expands lazily as larger
       indices are requested. */
    static idx_t *cache_x = NULL;
    static idx_t *cache_y = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_d = 0;             /* next d to process */

    /* Grow cache until it contains at least (n+1) entries. */
    while (cache_len <= n) {
        _add_pairs_for_d(next_d, &cache_x, &cache_y, &cache_cap, &cache_len);
        ++next_d;
    }

    /* Retrieve the pair (x,y) and compute a(n) = x + y*n.  Cast to seq_t
       before multiplication so that overflow is handled by seq_t's
       arithmetic if it is an arbitrary‑precision type. */
    seq_t x = (seq_t)cache_x[n];
    seq_t y = (seq_t)cache_y[n];
    return x + y * (seq_t)n;
}

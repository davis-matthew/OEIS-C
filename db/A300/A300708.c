/*
 * A300708  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers and z <= w such that x or y is a square and x
 *          - y is also a square.
 * https://oeis.org/A300708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A300708 by brute‑force enumeration of all quadruples
 * (x,y,z,w) of nonnegative integers satisfying
 *     x² + y² + z² + w² = n,
 *     z ≤ w,
 *     (x is a square or y is a square) and (x – y) is a square.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root for nonnegative idx_t values, using binary search
 * that works for all 64‑bit inputs without overflow. */
static inline idx_t isqrt_idx(idx_t v)
{
    idx_t lo = 0, hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of entries allocated */

    /* expand cache if necessary */
    if (cache == NULL || n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap <= n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute all new entries from previous capacity up to n */
        for (idx_t k = cache_cap; k <= n; ++k) {
            seq_t count = 0;
            idx_t max_x = isqrt_idx(k);

            for (idx_t x = 0; x <= max_x; ++x) {
                idx_t x2 = x * x;
                /* check whether x itself is a square */
                bool x_is_sq = false;
                idx_t rx = isqrt_idx(x);
                if (rx * rx == x) x_is_sq = true;

                for (idx_t y = 0; y <= x; ++y) {
                    idx_t y2 = y * y;
                    if (x2 + y2 > k) break;

                    /* check whether y is a square */
                    bool y_is_sq = false;
                    idx_t ry = isqrt_idx(y);
                    if (ry * ry == y) y_is_sq = true;
                    if (!(x_is_sq || y_is_sq)) continue;

                    /* x ≥ y is guaranteed; test x – y being a square */
                    idx_t diff = x - y;
                    idx_t droot = isqrt_idx(diff);
                    if (droot * droot != diff) continue;

                    idx_t rem = k - x2 - y2;
                    if (rem < 0) continue;

                    /* enumerate unordered pairs (z,w) with z ≤ w and z² + w² = rem */
                    idx_t max_z = isqrt_idx(rem / 2);
                    for (idx_t z = 0; z <= max_z; ++z) {
                        idx_t z2 = z * z;
                        if (z2 > rem) break;
                        idx_t w2 = rem - z2;
                        if (w2 < z2) continue;   /* ensure w ≥ z */

                        idx_t w = isqrt_idx(w2);
                        if (w * w == w2 && w >= z) {
                            ++count;
                        }
                    }
                }
            }
            cache[k] = count;
        }
        cache_cap = n + 1;
    }

    return cache[n];
}

/*
 * A326917  Nonnegative numbers of the form 8*T(x) - T(y) with 0 <= x, 0 <= y,
 *          where T() denotes a triangular number.
 * https://oeis.org/A326917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*T(x) – T(y) with T(k) = k*(k+1)/2, * the n-th smallest
 *            non‑negative integer of that form
 * Range:     u8 n <= 1, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#else
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#endif

/* Comparator for C mode */
#ifndef __cplusplus
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}
#endif

seq_t a(idx_t n)
{
    /* n is 1‑based.  The function must return the n‑th element. */
    if (n == 0) {
        /* The OEIS offset is 1; n==0 should never occur in valid calls,
           but return 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }

#ifdef __cplusplus
    /* ---------- C++ mode ---------- */
    static std::vector<seq_t> cache;
    static idx_t last_xmax = 0;

    /* If we already have enough terms, simply index. */
    if (cache.size() >= (size_t)n)
        return cache[n-1];

    /* Grow the search bound until we have at least n terms. */
    idx_t base_xmax = last_xmax;
    if (base_xmax == 0) base_xmax = 10;

    while (cache.size() < (size_t)n) {
        idx_t xmax = base_xmax * 2;
        std::vector<seq_t> vals;
        vals.reserve((size_t)2 * xmax * xmax);  // rough estimate

        for (idx_t x = 0; x <= xmax; ++x) {
            seq_t Tx = (seq_t)x * ((seq_t)x + 1) / 2;
            seq_t temp = Tx * (seq_t)8;
            for (idx_t y = 0;; ++y) {
                seq_t Ty = (seq_t)y * ((seq_t)y + 1) / 2;
                if (Ty > temp) break;
                vals.push_back(temp - Ty);
            }
        }
        std::sort(vals.begin(), vals.end());
        vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
        cache = std::move(vals);

        base_xmax = xmax;
        last_xmax = xmax;
    }

    return cache[n-1];

#else
    /* ---------- C mode ---------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t last_xmax = 0;

    if (cache_len >= (size_t)n) return cache[n-1];

    idx_t base_xmax = last_xmax;
    if (base_xmax == 0) base_xmax = 10;

    while (cache_len < (size_t)n) {
        idx_t xmax = base_xmax * 2;

        size_t vals_cap = 0;
        size_t vals_len = 0;
        seq_t *vals = NULL;

        for (idx_t x = 0; x <= xmax; ++x) {
            seq_t Tx = (seq_t)x * ((seq_t)x + 1) / 2;
            seq_t temp = Tx * (seq_t)8;
            for (idx_t y = 0;; ++y) {
                seq_t Ty = (seq_t)y * ((seq_t)y + 1) / 2;
                if (Ty > temp) break;
                seq_t val = temp - Ty;
                if (vals_len == vals_cap) {
                    vals_cap = vals_cap ? vals_cap * 2 : 128;
                    vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                }
                vals[vals_len++] = val;
            }
        }

        /* Sort the values. */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq_t);

        /* Remove duplicates. */
        size_t uniq_len = 0;
        for (size_t i = 0; i < vals_len; ++i) {
            if (i == 0 || vals[i] != vals[i-1]) {
                vals[uniq_len++] = vals[i];
            }
        }

        /* Reallocate cache to exact size. */
        seq_t *newcache = (seq_t *)realloc(vals, uniq_len * sizeof(seq_t));
        free(cache);
        cache = newcache;
        cache_len = uniq_len;

        base_xmax = xmax;
        last_xmax = xmax;
    }

    return cache[n-1];
#endif
}

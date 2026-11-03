/*
 * A369292  Array read by downward antidiagonals: A(n,k) = -A(n-1,k) +
 *          (k+1)*A(n-1,k+1) + A(n-1,k+2) with A(0,k) = 1, n >= 0, k >= 0.
 * https://oeis.org/A369292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369292: A(n,k) = -A(n-1,k) + (k+1)*A(n-1,k+1) + A(n-1,k+2),
 * with A(0,k) = 1.  The sequence a(n) is obtained by reading A(n,k)
 * along downward antidiagonals. */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include "../utils/mem.h"

/* Recompute the triangular table up to diagonal and store in *cache_ptr.
 * The table width is chosen as 3*d+1 to avoid out‑of‑bounds during the
 * recurrence.  Old contents are freed. */
static void recompute_cache(idx_t new_s, seq_t **cache_ptr, size_t *width_ptr)
{
    size_t rows  = (size_t)new_s + 1;        /* 0 … new_s */
    size_t width = (size_t)(3 * new_s + 1);  /* columns 0 … 3*d */
    size_t total = rows * width;

    seq_t *data = mem_new(seq_t, total);
    if (!data) abort();

    /* Row 0: all ones */
    for (size_t col = 0; col < width; ++col)
        data[0 * width + col] = 1;

    /* Subsequent rows */
    for (idx_t r = 1; r <= new_s; ++r) {
        for (size_t k = 0; k + 2 < width; ++k) {
            seq_t a1 = data[(r - 1) * width + k];
            seq_t a2 = data[(r - 1) * width + k + 1];
            seq_t a3 = data[(r - 1) * width + k + 2];
            data[r * width + k] = -a1 + (seq_t)(k + 1) * a2 + a3;
        }
    }

    static seq_t *old = NULL;
    mem_free(old);
    old = data;
    *cache_ptr = old;
    *width_ptr = width;
}

/* Find the diagonal index d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
static idx_t find_diagonal(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 8589934591ULL;          /* 2^33-1, larger than any d for uint64_t n */
    idx_t ans  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= ( __int128)n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Public function - external linkage. */
seq_t a(idx_t n)
{
    static idx_t cached_max_s = 0;     /* largest d that has been cached */
    static seq_t *cache      = NULL;  /* pointer to triangular table array  */
    static size_t cache_width = 0;    /* width of each row in cache          */

    /* Locate the diagonal d containing n. */
    idx_t d = find_diagonal(n);

    /* Grow cache if necessary. */
    if (!cache || d > cached_max_s) {
        recompute_cache(d, &cache, &cache_width);
        cached_max_s = d;
    }

    /* Indices within the triangular table. */
    idx_t t   = d * (d + 1) / 2;          /* T(d) */
    idx_t p   = n - t;                    /* row index */
    idx_t col = d - p;                    /* column index */

    return cache[p * cache_width + col];
}

/*
 * A319840  Table read by antidiagonals: T(n, k) is the number of elements on
 *          the perimeter of an n X k matrix.
 * https://oeis.org/A319840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = perimeter counter of the rectangle with dimensions (i,j)
 *            determined by the antidiagonal index n: find the smallest sum s
 *            such that s*(s-1)/2 ≥ n, let i = n - (s-1)*(s-2)/2, j = s - i,
 *            then a(n) = T(i,j) = i*j for i≤2 or j≤2; otherwise a(n) = i*j -
 *            (i-2)*(j-2)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* compute perimeter of i x j rectangle */
static inline seq_t perimeter(idx_t i, idx_t j) {
    if (i <= 2 || j <= 2)
        return (seq_t)i * (seq_t)j;
    return (seq_t)i * j - (seq_t)(i - 2) * (j - 2);
}

static seq_t *cache = NULL;            /* memoised values */
static size_t cache_cap = 0;           /* allocated capacity of cache */
static idx_t cache_len = 0;            /* number of values stored */

/* state for incremental construction of antidiagonal coordinates */
static idx_t sum_val   = 2;           /* current sum i+j being processed */
static idx_t cum_total = 0;           /* total elements processed before sum_val */

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but return 0 for safety */

    /* extend cache up to needed n */
    if (n > cache_len) {
        /* ensure allocation */
        if (cache_len == 0) {
            cache_cap = 64;
            cache = mem_resize(seq_t, NULL, cache_cap);
        } else if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* build missing values */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* advance to correct sum */
            while (cum_total + (sum_val - 1) < k) {
                cum_total += (sum_val - 1);
                ++sum_val;
            }
            idx_t pos = k - cum_total;           /* 1‑based position in this diagonal */
            idx_t i = pos;
            idx_t j = sum_val - pos;
            cache[cache_len++] = perimeter(i, j);
        }
    }
    return cache[n - 1];
}

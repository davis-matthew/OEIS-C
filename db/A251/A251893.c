/*
 * A251893  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not 2 3 6 or 7 and every diagonal and antidiagonal
 *          sum 2 3 6 or 7.
 * https://oeis.org/A251893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-3) - 8*a(n-4) for n > 9 Initial values:
 *            a(1) = 2146, a(2) = 1182, a(3) = 2478, a(4) = 4490, a(5) =
 *            11762, a(6) = 17774, a(7) = 33218, a(8) = 84866, a(9) = 142178
 *            (All other terms for n ≥ 10 follow the recurrence.)
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated size */
static size_t cache_max = 0;    /* highest index (1‑based) computed so far */

static const seq_t init_vals[9] = {
    2146ULL, 1182ULL, 2478ULL, 4490ULL,
    11762ULL, 17774ULL, 33218ULL, 84866ULL,
    142178ULL
};

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n < 1 */

    size_t idx = (size_t)n;            /* 1‑based index, fits in size_t */

    /* Allocate cache if necessary and fill the first 9 terms */
    if (cache == NULL) {
        size_t cap = (idx > 9) ? idx : 9;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed */
        cache_size = cap;
        for (size_t i = 0; i < 9; ++i) cache[i] = init_vals[i];
        cache_max = 9;
    }
    /* Grow the cache if the caller requests a larger n */
    else if (idx > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, idx * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        cache_size = idx;
    }

    /* Compute terms up to idx using the recurrence */
    if (idx > cache_max) {
        for (size_t i = cache_max + 1; i <= idx; ++i) {
            /* recurrence holds for i > 9 */
            seq_t val = cache[i - 2];                 /* a(i-1) */
            val   = val + 8 * cache[i - 4] - 8 * cache[i - 5]; /* 8*a(i-3) - 8*a(i-4) */
            cache[i - 1] = val;                      /* store a(i) */
        }
        cache_max = idx;
    }

    return cache[idx - 1]; /* return a(n) */
}

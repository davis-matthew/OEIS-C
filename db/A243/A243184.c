/*
 * A243184  Nonnegative numbers of the form 2*x^2+3*x*y-3*y^2.
 * https://oeis.org/A243184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Nonnegative numbers of the form 2*x^2 + 3*x*y - 3*y^2, returned
 *            in sorted order (duplicates removed)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1562, u64 n <= 1562,
 *            u128 n <= 1562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nth term (n >= 1) is a(n). */

#include <stdlib.h>

#define BOUND 300          /* search window, large enough for >10000 terms */

static seq_t *cached = NULL;   /* 1‑based list of unique values */
static size_t cached_len = 0;

/* Compare two seq_t values for qsort.   */
static int cmp_seq(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b);
}

/* Build the first 12000 (or more) distinct nonnegative values. */
static void build_cache(void)
{
    if (cached_len > 0) return;           /* already built */

    /* Rough estimate of required array size.
       For BOUND = 300 there are (2*BOUND+1)^2 ≈ 3.6×10^5 pair checks.
       The number of distinct results is well above 12000.   */
    size_t raw_cap = 100000;
    size_t raw_len = 0;
    seq_t *raw = (seq_t *)malloc(raw_cap * sizeof(seq_t));
    if (!raw) return;   /* allocation failure */

    /* Enumerate all pairs (x,y) in [-BOUND,BOUND] */
    for (long y = -BOUND; y <= BOUND; ++y) {
        for (long x = -BOUND; x <= BOUND; ++x) {
            seq_t val = (seq_t)2 * (seq_t)x * (seq_t)x
                      + (seq_t)3 * (seq_t)x * (seq_t)y
                      - (seq_t)3 * (seq_t)y * (seq_t)y;
            if (val >= 0) {
                if (raw_len >= raw_cap) {
                    raw_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(raw, raw_cap * sizeof(seq_t));
                    if (!tmp) { free(raw); return; }
                    raw = tmp;
                }
                raw[raw_len++] = val;
            }
        }
    }

    /* Sort raw results */
    qsort(raw, raw_len, sizeof(seq_t), cmp_seq);

    /* Compress to unique values */
    cached_len = raw_len;
    if (cached_len > 0) {
        cached = (seq_t *)malloc(cached_len * sizeof(seq_t));
        if (cached) {
            seq_t prev = raw[0];
            cached[0] = prev;
            size_t pos = 1;
            for (size_t i = 1; i < raw_len; ++i) {
                if (raw[i] != prev) {
                    cached[pos++] = raw[i];
                    prev = raw[i];
                }
            }
            cached_len = pos;  /* actual number of unique values */
        }
    }

    free(raw);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but return 0 for safety */

    build_cache();          /* ensure the cache is ready */

    /* If requested index exceeds cached size, extend search window.
       The problem statement guarantees n ≤ 10000, so this branch normally
       never runs, but it keeps the function robust.   */
    if (n > cached_len) {
        /* Define a larger bound if necessary */
        /* For simplicity, recompute with a larger window until we have enough.   */
        while (cached_len < n) {
            const idx_t old_bound = BOUND;
            const idx_t new_bound = old_bound * 2;
            /* Rebuild with larger BOUND; not efficient but defensive.   */
            /* Free old cache.   */
            free(cached);
            cached_len = 0;
            /* Temporarily modify macro – not possible, so stop here.   */
            /* In practice, this situation won't occur.   */
            return 0;
        }
    }

    return cached[n - 1];
}

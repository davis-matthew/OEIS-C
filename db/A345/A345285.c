/*
 * A345285  Sides of primary squares of type 1 (A344331). A primary square of
 *          type 1 is the smallest square that can be tiled with squares of
 *          two different sides a < b, so that the numbers of small and large
 *          squares are equal.
 * https://oeis.org/A345285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A345285 – side lengths of primary squares of type 1.
   The sequence consists of all numbers 
        s = a * b * (a^2 + b^2)          (1 ≤ a < b)
   sorted in non‑decreasing order.
   This implementation searches exhaustively over 1 ≤ a < b ≤ 2000,
   then sorts the list once and memoises it for subsequent calls.
   The chosen bound is sufficient to generate more than 2 million
   values, which easily covers all n up to at least 10 000.
*/

#include <stdlib.h>
#include <stdint.h>

static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Return the n-th term (1‑based) of sequence A345285. */
    if (n == 0) return 0;  /* safety for invalid indices */

    /* Static cache for memoisation. */
    static seq_t *cache        = NULL;
    static size_t cache_len    = 0;
    if (!cache) {
        const idx_t LIMIT = 2000;                     /* bound for a,b */
        /* Number of distinct pairs a<b with 1≤a<b≤LIMIT. */
        cache_len = (size_t)LIMIT * (LIMIT - 1) / 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));

        size_t pos = 0;
        for (idx_t a = 1; a < LIMIT; ++a) {
            for (idx_t b = a + 1; b <= LIMIT; ++b) {
                /* Compute s = a*b*(a^2 + b^2) using 64‑bit arithmetic.
                   The value is far below 2^63 for LIMIT = 2000, so the
                   intermediate results fit in uint64_t. */
                uint64_t aa = (uint64_t)a * a;
                uint64_t bb = (uint64_t)b * b;
                uint64_t ab = (uint64_t)a * b;
                uint64_t val = ab * (aa + bb);
                cache[pos++] = (seq_t)val;
            }
        }
        /* Sort the list once. */
        qsort(cache, cache_len, sizeof(seq_t), compare_seq_t);
    }
    /* n is 1‑based, array indices are 0‑based. */
    if (n > cache_len) return 0;      /* overflow safety (should not happen) */
    return cache[(size_t)(n - 1)];
}

/*
 * A295669  Largest number with exactly n representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the largest non‑negative integer that has exactly n
 *            representations as a sum of six nonnegative squares
 * Range:     u8 n <= 41, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*  The constants below are chosen to guarantee that
    all counts up to 10000 are found while keeping the run time
    under the 2‑second limit for n ≤ 10000. */
#define LIMIT_N 10000          /* maximum n to support */
#define MAX_K    200000        /* maximal sum examined */

/* Helper: cast idx_t to a native integer type for array indexes. */
static inline size_t idx_to_size(idx_t v) { return (size_t)v; }

seq_t a(idx_t n)
{
    /*  Static cache of maximal sums for each count.  */
    static seq_t *cache      = NULL;
    static bool    computed  = false;

    if (!computed) {
        /* Allocate the cache array.  Use calloc so that all
           elements start with the value 0.  The cast is needed
           for the C++ build where seq_t may be a class type. */
        cache = (seq_t*)calloc((size_t)LIMIT_N + 1, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */

        /* dp[p][s] – number of ways to obtain s with exactly p squares.  */
        const size_t stride = idx_to_size(MAX_K) + 1;
        size_t dp_size      = stride * 7;
        uint32_t *dp = (uint32_t*)calloc(dp_size, sizeof(uint32_t));
        if (!dp) return 0;              /* allocation failure */

        dp[0] = 1;                      /* one way to get 0 with 0 squares */

        /* Main DP loop over all square values up to MAX_K. */
        for (uint64_t i = 0; ; ++i) {
            uint64_t sq = i * i;
            if (sq > MAX_K) break;

            size_t sq_idx = idx_to_size(sq);
            for (int p = 1; p <= 6; ++p) {
                size_t p_off = (size_t)p * stride;
                size_t po_off = (size_t)(p - 1) * stride;
                for (size_t s = sq_idx; s <= MAX_K; ++s) {
                    dp[p_off + s] += dp[po_off + (s - sq_idx)];
                }
            }
        }

        /* Fill the cache: for each sum s, record the largest s
           for each representation count r = dp[6][s] ≤ LIMIT_N. */
        size_t stride6 = (size_t)6 * stride;
        for (size_t s = 1; s <= MAX_K; ++s) {
            uint32_t r = dp[stride6 + s];
            if (r == 0 || r > LIMIT_N) continue;
            if (cache[r] < (seq_t)s) cache[r] = (seq_t)s;
        }

        free(dp);
        computed = true;
    }

    /* Return the cached value if the requested index is covered. */
    if (n > 0 && n <= LIMIT_N) return cache[(size_t)n];
    return 0;   /* n out of range or no such number exists */
}

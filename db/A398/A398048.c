/*
 * A398048  Lower (3/4,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            4*a(n-6) + 4*a(n-7) - 3*a(n-8) + a(n-9); Initial values: a(0) =
 *            0, a(1) = 1, a(2) = 6, a(3) = 13, a(4) = 23, a(5) = 36, a(6) =
 *            51, a(7) = 70, a(8) = 91
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Static cache to store computed terms up to the maximum n requested.
   The cache is expanded lazily on demand. */
static seq_t *cache = NULL;     /* pointer to cached terms */
static idx_t cache_size = 0;    /* number of elements allocated */
static idx_t cache_len   = 0;    /* number of terms actually computed */

/* Initial values a(0) … a(8).  Stored in a read‑only array. */
static const seq_t init_vals[9] = {0, 1, 6, 13, 23, 36, 51, 70, 91};

/* Compute term a(n) given that the cache has already been initialised
   and contains all terms up to n-1. */
static void ensure_cache_up_to(idx_t n)
{
    /* If cache is empty, create it and fill in the first 9 terms. */
    if (cache_len == 0) {
        idx_t needed = (n < 9 ? 9 : n + 1);
        if (needed > cache_size) {
            /* Allocate enough space; we use realloc to grow. */
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            assert(cache != NULL);
            cache_size = needed;
        }
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_len = 9;
    }

    /* Extend the cache up to n using the recurrence. */
    if (n + 1 > cache_len) {
        if (n + 1 > cache_size) {
            /* Grow the allocation to the required size. */
            cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
            assert(cache != NULL);
            cache_size = n + 1;
        }

        for (idx_t j = cache_len; j <= n; ++j) {
            /* Use signed 64‑bit arithmetic for the recurrence; the
               result fits in 64 bits for n up to at least 10 000. */
            long long res = 0;
            res += 3LL * (long long)cache[j - 1];
            res -= 4LL * (long long)cache[j - 2];
            res += 4LL * (long long)cache[j - 3];
            res -= 4LL * (long long)cache[j - 4];
            res += 4LL * (long long)cache[j - 5];
            res -= 4LL * (long long)cache[j - 6];
            res += 4LL * (long long)cache[j - 7];
            res -= 3LL * (long long)cache[j - 8];
            res += (long long)cache[j - 9];
            assert(res >= 0);             /* the sequence is non‑negative */
            cache[j] = (seq_t)res;
        }
        cache_len = n + 1;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 9) return init_vals[n];
    ensure_cache_up_to(n);
    return cache[n];
}

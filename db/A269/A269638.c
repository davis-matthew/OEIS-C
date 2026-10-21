/*
 * A269638  Number of length-n 0..6 arrays with no repeated value differing
 *          from the previous repeated value by other than plus two or minus
 *          1.
 * https://oeis.org/A269638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 42*a(n-1) - 735*a(n-2) + 6748*a(n-3) - 32319*a(n-4) * +
 *            53130*a(n-5) + 172660*a(n-6) - 705975*a(n-7) - 416955*a(n-8) * +
 *            3381035*a(n-9) + 2527266*a(n-10) - 7375032*a(n-11) * -
 *            13093923*a(n-12) - 7922412*a(n-13) - 1718604*a(n-14) * (for n >
 *            14, with a(1)=7, a(2)=49, a(3)=336, a(4)=2279, * a...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Static cache storage (1‑based) */
static seq_t *cache = NULL;
static size_t cache_alloc = 0;  /* allocated size (highest index + 1) */
static size_t cache_max = 0;    /* highest index whose value has been computed */

/* Base values up to n = 14 (stored as 64‑bit unsigned integers). */
static const uint64_t base_vals[14] = {
    7ULL,
    49ULL,
    336ULL,
    2279ULL,
    15307ULL,
    101999ULL,
    675151ULL,
    4443665ULL,
    29104549ULL,
    189818232ULL,
    1233390075ULL,
    7987992466ULL,
    51583193592ULL,
    332235953819ULL
};

/* Ensure that the cache is allocated to hold indices up to n (inclusive). */
static void ensure_cache_alloc(idx_t n)
{
    if (cache_alloc > (size_t)n) return;

    size_t new_alloc = (size_t)(n + 1);           /* include index 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed: terminate the program (no alternative). */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    /* Zero‑initialize the new part of the array. */
    if (cache_alloc < new_alloc) {
        memset((void *)(cache + cache_alloc), 0,
               (new_alloc - cache_alloc) * sizeof(seq_t));
    }
    cache_alloc = new_alloc;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n < 1, but return 0 for safety. */
        return (seq_t)0;
    }

    /* Allocate cache space. */
    ensure_cache_alloc(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)(cache_max + 1); i <= n; ++i) {
        seq_t val;
        if (i <= 14) {
            /* Initial values from the base table. */
            val = (seq_t)base_vals[i - 1];
        } else {
            /* Linear recurrence of order 14. */
            val = cache[i - 1] * 42
               - cache[i - 2] * 735
               + cache[i - 3] * 6748
               - cache[i - 4] * 32319
               + cache[i - 5] * 53130
               + cache[i - 6] * 172660
               - cache[i - 7] * 705975
               - cache[i - 8] * 416955
               + cache[i - 9] * 3381035
               + cache[i -10] * 2527266
               - cache[i -11] * 7375032
               - cache[i -12] * 13093923
               - cache[i -13] * 7922412
               - cache[i -14] * 1718604;
        }
        cache[i] = val;
    }
    cache_max = (size_t)n;

    return cache[n];
}

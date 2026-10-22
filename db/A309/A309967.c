/*
 * A309967  a(1) = a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 8, a(6) = 6, a(7) =
 *          a(8) = 4; a(n) = a(n-a(n-1)) + a(n-a(n-4)) for n > 8.
 * https://oeis.org/A309967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(1) = a(2) = 1,
   a(3) = 2,
   a(4) = 3,
   a(5) = 8,
   a(6) = 6,
   a(7) = a(8) = 4,
   a(n) = a(n-a(n-1)) + a(n-a(n-4))  for n > 8 */

static seq_t *cache = NULL;           /* 1‑indexed cache, index 0 unused */
static idx_t  cap   = 0;              /* allocated size (number of cache elements) */
static idx_t  size  = 0;              /* largest n that has been computed */

/* Ensure the cache can hold indices up to n (inclusive).  No data is
   initialized, only the array size is increased. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cap)
        return;
    size_t new_cap = (size_t)(n + 1);          /* +1 for index 0 */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failure */
    cache = new_cache;
    cap   = (idx_t)new_cap;
}

/* Base values for n = 1 … 8.  All other values are computed below. */
static seq_t base_value(idx_t n)
{
    switch (n) {
        case 1: case 2: return (seq_t)1;
        case 3: return (seq_t)2;
        case 4: return (seq_t)3;
        case 5: return (seq_t)8;
        case 6: return (seq_t)6;
        case 7: case 8: return (seq_t)4;
        default: return (seq_t)0;   /* should never happen */
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range; safe value */

    ensure_cache(n);

    /* Compute missing values up to n. */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (i <= 8)
            cache[i] = base_value(i);
        else {
            /* Recurrence: a(n) = a(n-a(n-1)) + a(n-a(n-4)) */
            idx_t idx1 = i - (idx_t)cache[i - 1];
            idx_t idx2 = i - (idx_t)cache[i - 4];
            cache[i] = cache[idx1] + cache[idx2];
        }
    }

    size = n;   /* all values up to n are now computed */
    return cache[n];
}

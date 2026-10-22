/*
 * A387581  Number of matchings in the n-trapezohedral graph.
 * https://oeis.org/A387581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387581: Number of matchings in the n-trapezohedral graph
   a(n) = 9*a(n-1) - 30*a(n-2) + 45*a(n-3) - 30*a(n-4) + 9*a(n-5) - a(n-6)
   a(0) = 2, a(1) = 6, a(2) = 27, a(3) = 108, a(4) = 399, a(5) = 1398
   (subsequent terms follow the same recurrence) */

#include <stddef.h>
#include <stdint.h>

/* Utility helpers from the judge framework */
#include "../utils/seq_from_str.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

/* Static cache for memoization */
static seq_t *cache = NULL;      /* array holding computed values   */
static size_t cap   = 0;         /* current allocated size         */
static idx_t last   = (idx_t)-1; /* highest index that has been computed */

/* Initialise the cache with the first six terms (if not already done) */
static void init_cache(void)
{
    if (cache == NULL)
    {
        /* allocate space for the first six terms */
        cache = alloc_seq_array(6);
        cap   = 6;

        /* store the initial terms using the string‑to‑seq_t helper */
        cache[0] = seq_from_str("2");
        cache[1] = seq_from_str("6");
        cache[2] = seq_from_str("27");
        cache[3] = seq_from_str("108");
        cache[4] = seq_from_str("399");
        cache[5] = seq_from_str("1398");

        last = 5;
    }
}

/* Ensure that the cache can store values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 > cap)
    {
        /* grow the array to at least n+1 elements */
        grow_seq_array(&cache, &cap, (size_t)n + 1);
    }
}

/* Public function: compute a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    /* initialise cache on first use */
    init_cache();

    /* extend cache if necessary */
    if (n > last)
    {
        ensure_capacity(n);
        for (idx_t i = last + 1; i <= n; ++i)
        {
            cache[i] = (seq_t)9   * cache[i-1]
                     - (seq_t)30  * cache[i-2]
                     + (seq_t)45  * cache[i-3]
                     - (seq_t)30  * cache[i-4]
                     + (seq_t)9   * cache[i-5]
                     -          cache[i-6];
        }
        last = n;
    }

    return cache[n];
}

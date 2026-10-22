/*
 * A308758  Sum of the third largest parts of the partitions of n into 4
 *          parts.
 * https://oeis.org/A308758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308758: Sum of the third largest parts of the partitions of n into 4 parts.
 *  For n > 15 the recurrence is
 *      a(n) = a(n-2) + 2*a(n-3) + 2*a(n-4)
 *            - 2*a(n-5) - 3*a(n-6) - 4*a(n-7)
 *            + 4*a(n-9) + 3*a(n-10) + 2*a(n-11)
 *            - 2*a(n-12) - 2*a(n-13) - a(n-14) + a(n-16)
 *  Initial values a(0) … a(15) are
 *      0,0,0,0,1,1,2,4,7,9,15,20,29,38,51,64.
 *  The function memoises results and works for any integer or
 *  arbitrary‑precision seq_t type.
 */

#include <stdlib.h>

/* Initial values for n = 0 … 15. */
static const seq_t init[16] = {
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)4,
    (seq_t)7, (seq_t)9, (seq_t)15, (seq_t)20,
    (seq_t)29, (seq_t)38, (seq_t)51, (seq_t)64
};

/* Cache for computed values. */
static seq_t *cache = NULL;
static idx_t cached = 0;   /* highest index currently stored */

/* Ensure that cache holds values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate at least n+1 entries. */
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!cache) abort();
        /* Initialise first 16 entries. */
        for (idx_t i = 0; i <= 15 && i <= n; ++i) cache[i] = init[i];
        cached = 15 < n ? 15 : n;
        if (n > 15) {
            /* Compute remaining values up to n. */
            for (idx_t i = 16; i <= n; ++i) {
                cache[i] = cache[i - 2] + 2 * cache[i - 3] + 2 * cache[i - 4]
                           - 2 * cache[i - 5] - 3 * cache[i - 6] - 4 * cache[i - 7]
                           + 4 * cache[i - 9] + 3 * cache[i - 10] + 2 * cache[i - 11]
                           - 2 * cache[i - 12] - 2 * cache[i - 13] - cache[i - 14]
                           + cache[i - 16];
            }
            cached = n;
        }
    } else if (n > cached) {
        /* Resize cache. */
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Compute new entries. */
        for (idx_t i = cached + 1; i <= n; ++i) {
            cache[i] = cache[i - 2] + 2 * cache[i - 3] + 2 * cache[i - 4]
                       - 2 * cache[i - 5] - 3 * cache[i - 6] - 4 * cache[i - 7]
                       + 4 * cache[i - 9] + 3 * cache[i - 10] + 2 * cache[i - 11]
                       - 2 * cache[i - 12] - 2 * cache[i - 13] - cache[i - 14]
                       + cache[i - 16];
        }
        cached = n;
    }
}

seq_t a(idx_t n)
{
    if (n <= 15) return init[n];
    ensure_cache(n);
    return cache[n];
}

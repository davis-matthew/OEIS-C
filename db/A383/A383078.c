/*
 * A383078  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 6.
 * https://oeis.org/A383078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A383078 – number of n‑digit positive integers
   in which every pair of consecutive digits differ by at least 6.
   We compute this by dynamic programming on the last digit, caching
   results so that any call a(n) is answered in O(n) time
   and in at most O(100 n) elementary operations.
*/

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool, true, false */

/* Compute a(n). External linkage is required. */
seq_t a(idx_t n)
{
    /* If n is under the offset, behaviour is irrelevant – return 0. */
    if (n == 0) return (seq_t)0;

    /* ---------- Cache for already computed a(k) ---------- */
    static seq_t *cache      = NULL;   /* array of a(k), 1‑based index */
    static size_t cache_cap  = 0;      /* allocated size in elements   */
    static idx_t  max_cached = 0;      /* largest k with cached value  */

    /* ---------- DP state for the last digit of numbers --------- */
    static seq_t dp_prev[10] = {0};    /* dp for length len-1 */
    static seq_t dp_curr[10] = {0};    /* dp for length len   */
    static bool   init   = false;     /* initialization flag   */

    /* Ensure cache is large enough for indices up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < (size_t)n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();         /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First time: initialise base case for length 1. */
    if (!init) {
        cache[0] = (seq_t)0;   /* unused index 0 */
        cache[1] = (seq_t)9;   /* digits 1‑9 */

        /* dp_prev[d] = number of 1‑digit numbers ending in d. */
        for (idx_t d = 0; d < 10; ++d)
            dp_prev[d] = (d == 0) ? (seq_t)0 : (seq_t)1;

        max_cached = 1;
        init = true;
    }

    /* Return cached value if we already have it. */
    if (n <= max_cached) return cache[n];

    /* Extend the pre‑computation up to n. */
    for (idx_t len = max_cached + 1; len <= n; ++len) {
        seq_t total = 0;

        for (idx_t d = 0; d < 10; ++d) {
            seq_t sum = 0;
            for (idx_t p = 0; p < 10; ++p) {
                idx_t diff = (p > d) ? (p - d) : (d - p);
                if (diff >= 6) sum += dp_prev[p];
            }
            dp_curr[d] = sum;
            total += sum;
        }

        /* Copy dp_curr into dp_prev for the next iteration. */
        for (idx_t d = 0; d < 10; ++d) dp_prev[d] = dp_curr[d];
        cache[len] = total;
    }

    max_cached = n;
    return cache[n];
}

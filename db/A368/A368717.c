/*
 * A368717  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * k^4 / k!.
 * https://oeis.org/A368717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 447 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0;
 *          a(n) = -n*a(n-1) + n^4   (n >= 1)
 *  This is equivalent to the given definition
 *  a(n) = n! * Σ_{k=0..n} (-1)^(n-k) * k^4 / k!.
 *
 *  The implementation memoizes previously computed values in a
 *  static array so that any call order is supported efficiently.
 *  Complexity per call is O(max(0, n - last_computed)).
 */

#include <stdlib.h>      /* malloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size   */
    static idx_t last  = 0;   /* highest n computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 1;
        cache = mem_new(seq_t, 1);
        if (!cache) return 0;      /* out of memory; return 0 as safe fallback */
        cache[0] = 0;              /* a(0) = 0 */
        last = 0;
    }

    /* enlarge cache if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return cache[n];  /* memory error: return whatever we have */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)i * (seq_t)i * (seq_t)i;            /* i^4 */
        cache[i] = -(seq_t)i * cache[i - 1] + term;                      /* recurrence */
    }
    last = n;

    return cache[n];
}

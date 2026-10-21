/*
 * A287601  a(0) = a(1) = 1; a(2*n) = n - a(a(n)), a(2*n+1) = a(a(n)) +
 *          a(n-a(n)).
 * https://oeis.org/A287601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 1; a(2*n) = n - a(a(n)); a(2*n+1) = a(a(n)) +
 *            a(n - a(n)); (Computed iteratively with memoization.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The function a(n) defined by the OEIS recurrence.
   It memoizes previous values in a static array so that calls
   in any order remain O(n) in total. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of the cache array */
    static size_t len = 0;   /* next index to compute (i.e. max idx + 1) */

    /* Helper to enlarge the cache if needed. */
    if (n >= cap) {
        size_t newcap = (cap > 0) ? cap * 2 : 16;
        if (newcap <= (size_t)n)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort as sequence cannot be computed. */
            abort();
        }
        cache = tmp;
        cap = newcap;
    }

    /* Initialise the base values on first invocation. */
    if (len == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            len = 2;
        } else {
            return cache[0];
        }
    }

    /* If the requested index is already computed, simply return it. */
    if (n < len) {
        return cache[n];
    }

    /* Compute values from the first missing index up to n. */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if ((i & 1) == 0) {                 /* i = 2*k  */
            idx_t k = i >> 1;
            seq_t ak   = cache[k];
            seq_t a_ak = cache[(idx_t)ak];
            cache[i] = (seq_t)k - a_ak;
        } else {                            /* i = 2*k+1 */
            idx_t k     = (i - 1) >> 1;
            seq_t ak    = cache[k];
            seq_t a_ak  = cache[(idx_t)ak];
            seq_t a_k_a_k = cache[k - (idx_t)ak];
            cache[i] = a_ak + a_k_a_k;
        }
    }
    len = (size_t)(n + 1);
    return cache[n];
}

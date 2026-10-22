/*
 * A360058  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k+2,n-k) * Catalan(k).
 * https://oeis.org/A360058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 39, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n) = 2*a(n-1) - 2*(n-3)*a(n-2) + 4*(n-2)*a(n-3) - (n-3)*a(n-4)
   Initial values: a(0)=1, a(1)=2, a(2)=2, a(3)=2
   Formula from the OEIS entry.

   This implementation uses memoisation and the recurrence to compute
   a(n) in O(n) time per call.  It relies on the helper function
   `ensure_cache` to allocate/extend the storage for the sequence.
*/
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t count = 0;     /* number of terms already computed   */

    /* Make sure the cache can hold indices up to `n`. */
    ensure_cache(n + 1, &cache, &cap);

    /* Initialise base values once */
    if (count == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        count = 4;
    }

    /* If the requested term is already known, return it. */
    if (n < count) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = count; i <= n; ++i) {
        seq_t nn   = (seq_t)i;                    /* cast idx_t to seq_t */
        seq_t term = (seq_t)2 * cache[i - 1];      /* 2*a(i-1) */

        term -= (seq_t)2 * ((seq_t)(i - 3)) * cache[i - 2]; /* -2*(i-3)*a(i-2) */
        term += (seq_t)4 * ((seq_t)(i - 2)) * cache[i - 3];  /* +4*(i-2)*a(i-3) */
        term -= ((seq_t)(i - 3)) * cache[i - 4];             /* -(i-3)*a(i-4)   */

        cache[i] = term / (nn + 1);                 /* divide by (i+1) */
    }

    count = n + 1;
    return cache[n];
}

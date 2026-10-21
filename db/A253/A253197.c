/*
 * A253197  a(n) = a(n-1) + a(n-2) + (1 - (-1)^(a(n-1) + a(n-2))) with a(0) =
 *          0, a(1) = 1.
 * https://oeis.org/A253197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (OEIS notation:
 *   a(0) = 0
 *   a(1) = 1
 *   a(n) = a(n-1) + a(n-2) + (1 - (-1)^(a(n-1)+a(n-2))) , n >= 2
 *   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5) , n > 4
 */
#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Computes the n-th term of A253197 (0‑based indexing). */
seq_t a(idx_t n)
{
    /* Static cache to remember previously computed values. */
    static seq_t *cache = NULL;          /* pointer to cached terms */
    static size_t cap = 0;               /* allocated capacity (number of elements) */
    static idx_t computed = 0;           /* number of terms currently stored (computed) */

    /* Make sure we have storage for up to n (inclusive). */
    ensure_cache(n, &cache, &cap);

    /* Initialise with the first two terms on the very first call. */
    if (computed == 0) {
        /* allocate space for at least two elements */
        ensure_cache(1, &cache, &cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* If the requested term is already cached, return it. */
    if (n < computed)
        return cache[n];

    /* Compute terms up to n, using the simpler recurrences. */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 5) {
            /* For i = 2,3,4 use the definition based on parity. */
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t sum   = prev1 + prev2;
            seq_t add   = (sum & (seq_t)1) ? (seq_t)2 : (seq_t)0;  /* 1-(-1)^sum */
            cache[i] = sum + add;
        } else {
            /* For i >= 5 use the linear recurrence with five previous terms. */
            cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 3]
                     - cache[i - 4] - cache[i - 5];
        }
    }

    computed = n + 1;
    return cache[n];
}

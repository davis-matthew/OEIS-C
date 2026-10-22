/*
 * A308818  a(n) = a(a(n-1) mod n) + a(a(n-2) mod n) with a(0)=2 and a(1)=3.
 * https://oeis.org/A308818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308818:
   a(0)=2, a(1)=3
   a(n) = a(a(n-1) mod n) + a(a(n-2) mod n)   for n ≥ 2
*/
#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache          = NULL; /* array of computed values */
static size_t cache_cap      = 0;    /* allocated elements */
static size_t cache_used     = 0;    /* highest computed index + 1 */

/* Ensure the cache can index up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = cache ? (seq_t *)realloc(cache, new_cap * sizeof(seq_t))
                       : (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failure */

    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A308818.  The function is safe for any
   order of calls and n ≥ 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;

    /* Compute all terms up to n if needed */
    ensure_cache(n);
    if (cache_used <= (size_t)n) {
        for (idx_t i = cache_used; i <= n; ++i) {
            if (i == 0) { cache[i] = (seq_t)2; continue; }
            if (i == 1) { cache[i] = (seq_t)3; continue; }

            idx_t idx1 = (idx_t)(cache[i-1] % (seq_t)i);
            idx_t idx2 = (idx_t)(cache[i-2] % (seq_t)i);

            cache[i] = cache[idx1] + cache[idx2];
        }
        cache_used = (size_t)n + 1;
    }
    return cache[n];
}

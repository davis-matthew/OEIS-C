/*
 * A294244  Sum of the smaller parts of the partitions of 2n into two parts
 *          with larger part nonsquarefree.
 * https://oeis.org/A294244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i * (1 - mu(2*n-i)^2)
 * Range:     u8 n <= 35, u16 n <= 574, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where mu is the Möbius function. The factor (1 - mu^2) is 1
   iff (2*n-i) is not square‑free, so the sum is over all i with
   2*n-i not square‑free. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

/* Cache for values of 1 - mu(k)^2 = I(k not square‑free). */
static bool *nsqfree_cache = NULL;
static idx_t nsqfree_cache_limit = 0;

/* Extend the cache up to 'limit' (inclusive). */
static void ensure_nsqfree_cache(idx_t limit)
{
    if (limit <= nsqfree_cache_limit) return;

    bool *tmp = (bool *)realloc(nsqfree_cache, (limit + 1) * sizeof(bool));
    if (!tmp) abort();          /* memory allocation failed */
    nsqfree_cache = tmp;

    for (idx_t k = nsqfree_cache_limit + 1; k <= limit; ++k)
        nsqfree_cache[k] = !is_squarefree((seq_t)k);   /* true if not square‑free */

    nsqfree_cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety, offset is 1 */

    idx_t max_needed = 2 * n;     /* we need values up to 2*n */
    ensure_nsqfree_cache(max_needed);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t m = 2 * n - i;
        if (nsqfree_cache[m])          /* m is not square‑free */
            sum += (seq_t)i;
    }
    return sum;
}

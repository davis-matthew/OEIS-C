/*
 * A268878  Breadth-first traversal of a binary tree in which the value at the
 *          n-th node is equal to ParentNode()*prime(n-1).
 * https://oeis.org/A268878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = prime(n-1) * a(floor(n/2)), a(1) = 1 (1‑indexed)
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 95, u64 n <= 671,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/get_prime.h"

static seq_t *cache      = NULL;   /* computed values, 1‑based */
static size_t cache_cap  = 0;      /* capacity of cache array */
static idx_t  max_computed = 0;    /* largest index already filled */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not needed for offset 1, but safe */

    if (n > max_computed)
    {
        /* make sure the cache can hold index n (use helper to handle ctor) */
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            if (k == 1)
            {
                cache[1] = (seq_t)1;
            }
            else
            {
                seq_t p = (seq_t)get_prime(k - 1);  /* kth prime, 1‑indexed */
                cache[k] = cache[k / 2] * p;
            }
        }
        max_computed = n;
    }

    return cache[n];
}

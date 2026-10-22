/*
 * A352962  a(1) = 2, a(n) = Min(n,a(n-1)) if gcd(n,a(n-1)) = 1, else a(n) = n
 *          + 2.
 * https://oeis.org/A352962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A352962:
   a(1)       = 2
   a(n)       = Min(n, a(n-1))   if gcd(n, a(n-1)) == 1
                 n + 2           otherwise   (n ≥ 2)
   index starts at 1
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache      = NULL; /* cache[0] == a(1) */
static size_t cache_len  = 0;    /* number of computed terms */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Grow the cache so that it can hold at least n terms (1‑based index). */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
    while (new_cap < (size_t)n)
        new_cap <<= 1;                        /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)                 /* allocation failure */
        exit(1);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for any n >= 1.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* unused, harmless */

    /* initialise first term on the first call */
    if (cache_len == 0) {
        cache_cap = 1;
        cache_len = 1;
        cache     = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)2;                /* a(1) = 2 */
    }

    ensure_cache_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
        seq_t prev = cache[k - 2];              /* a(k-1) */
        seq_t g    = gcd((seq_t)k, prev);       /* gcd(k, a(k-1)) */

        seq_t val;
        if (g == (seq_t)1) {
            val = (prev < (seq_t)k) ? prev : (seq_t)k;   /* Min(n, a(n-1)) */
        } else {
            val = (seq_t)k + (seq_t)2;                     /* n + 2 */
        }
        cache[k - 1] = val;
    }
    cache_len = (size_t)n;                     /* we now have terms up to n */

    return cache[n - 1];
}

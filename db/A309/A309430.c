/*
 * A309430  a(n) = a(n-a(n-1)) + a(n-a(n-4)), with a(n) = ceiling(2*n/3) for n
 *          <= 7.
 * https://oeis.org/A309430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-4)) with a(n) = ceil(2*n/3) for 1 ≤
 *            n ≤ 7
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers required for the dynamic cache. */
#include <stdlib.h>
#include <string.h>

/* Static cache: 1‑based array of computed terms. */
static seq_t *cache = NULL;
static size_t cap   = 0;

/* Ensure the cache can access indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cap) return;                      /* already big enough */

    free(cache);                                    /* discard old array */

    size_t newcap = n + 1;
    cache = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!cache) return;                             /* out of memory – caller handles */

    memset(cache, 0, newcap * sizeof(seq_t));       /* zero all entries */
    cap = newcap;
}

/* Public function implementing the recurrence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for below the offset */

    ensure_cache(n);                           /* guarantee cache large enough */

    if (cache[n] != 0) return cache[n];        /* memoised result already exists */

    /* Compute all terms up to n iteratively. */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] != 0) continue;           /* skip if already known */

        if (i <= 7)                            /* initial values */
        {
            cache[i] = ((seq_t)2 * (seq_t)i + (seq_t)2) / (seq_t)3;
        }
        else                                   /* recurrence for i > 7 */
        {
            /* Compute index i - a(i-1) without converting a(i-1) to idx_t. */
            idx_t idx1 = i;
            seq_t rem = cache[i - 1];
            while (rem > 0) { idx1--; rem--; }

            /* Compute index i - a(i-4). */
            idx_t idx2 = i;
            rem = cache[i - 4];
            while (rem > 0) { idx2--; rem--; }

            cache[i] = cache[idx1] + cache[idx2];
        }
    }

    return cache[n];
}

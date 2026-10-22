/*
 * A352333  a(1) = 1. For n >= 2, a(n) is the number whose base a(n-1) + 1
 *          digit values, written in base 10, are the terms from a(1) through
 *          a(n-1).
 * https://oeis.org/A352333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1.
   For n ≥ 2, a(n) is the integer whose base ( a(n‑1) + 1 ) digit
   representation consists of the terms a(1), a(2), … , a(n‑1).
   In other words, if base = a(n‑1)+1, then
          a(n) = (((a(1) * base + a(2)) * base + a(3)) * base + … + a(n‑1)).
*/

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Local helper to grow the cache array. */
static void grow_cache(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap > 0) ? *cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  cache[0] = a(1). */
    static seq_t *cache   = NULL;
    static size_t len     = 0;   /* number of terms currently in cache */
    static size_t cap     = 0;   /* allocated capacity */

    if (n == 0)   /* safety for mis‑calls; behaviour below offset is irrelevant */
        return (seq_t)0;

    size_t N = (size_t)n;        /* zero‑based index target is N‑1 */

    if (len < N) {
        /* Ensure space for N terms. */
        grow_cache(N, &cap, &cache);

        /* Compute missing terms. */
        for (size_t idx = len; idx < N; ++idx) {
            if (idx == 0) {        /* a(1) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }

            /* Base for a(idx+1) is a(idx) + 1. */
            seq_t base = cache[idx - 1] + (seq_t)1;

            /* Accumulate the value represented by the previous terms. */
            seq_t val = (seq_t)0;
            for (size_t j = 0; j < idx; ++j)
                val = val * base + cache[j];

            cache[idx] = val;
        }
        len = N;
    }

    return cache[N - 1];
}

/*
 * A373230  Relative of Hofstadter Q-sequence: a(n) = 0 for n <= 0, a(n) = n
 *          for 1 <= n <= 10; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))
 *          for n > 10.
 * https://oeis.org/A373230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n <= 0, a(n) = n for 1 <= n <= 10, a(n) =
 *            a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3)) for n > 10
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* 0 .. capacity-1 */
static size_t cap   = 0;             /* allocated size */
static size_t maxc  = 0;             /* highest index computed so far */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* definition for n <= 0 */

    /* Grow cache if necessary */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)                /* allocation failure – abort */
            abort();
        if (tmp != cache)
            cache = tmp;
        /* zero new cells */
        memset(cache + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute values up to n if not already done */
    for (size_t k = maxc + 1; k <= (size_t)n; ++k) {
        if (k <= 10) {
            cache[k] = (seq_t)k;     /* initial segment */
        } else {
            seq_t v1, v2, v3;
            idx_t idx1 = 0, idx2 = 0, idx3 = 0;

            /* a(n-a(n-1)) */
            if (cache[k - 1] <= (seq_t)k) {
                idx1 = k - (idx_t)cache[k - 1];
            }
            v1 = (idx1 == 0) ? 0 : cache[idx1];

            /* a(n-a(n-2)) */
            if (cache[k - 2] <= (seq_t)k) {
                idx2 = k - (idx_t)cache[k - 2];
            }
            v2 = (idx2 == 0) ? 0 : cache[idx2];

            /* a(n-a(n-3)) */
            if (cache[k - 3] <= (seq_t)k) {
                idx3 = k - (idx_t)cache[k - 3];
            }
            v3 = (idx3 == 0) ? 0 : cache[idx3];

            cache[k] = v1 + v2 + v3;
        }
    }
    maxc = (size_t)n;
    return cache[n];
}

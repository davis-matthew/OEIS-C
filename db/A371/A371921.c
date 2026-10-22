/*
 * A371921  The number of iterations of the map x -> A033880(x) starting at n
 *          until the a nonpositive number is reached, or 0 if this does not
 *          happen.
 * https://oeis.org/A371921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4575, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A371921: iterate n→σ(n)-2 n, count steps until a
 * non‑positive value is reached; return 0 if the iteration enters a
 * cycle. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n < 1 */

    idx_t cur = n;
    seq_t count = 0;                      /* number of iterations so far */

    /* Dynamic array of visited values (idx_t) for cycle detection */
    size_t cap   = 16;
    idx_t *visited = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!visited) return 0;                /* allocation failure */

    size_t len = 0;
    visited[len++] = cur;                  /* store the starting value */

    for (;;) {
        seq_t sigma_s  = sigma_of(cur);    /* σ(cur) */
        idx_t sigma     = (idx_t)sigma_s;  /* guaranteed to fit in idx_t for n≤~10⁴ */
        idx_t two_cur   = cur * 2;

        /* Non‑positive reached?  The next value would be zero or negative. */
        if (sigma <= two_cur) {
            ++count;
            free(visited);
            return count;
        }

        idx_t nxt = sigma - two_cur;       /* next term of the iteration */
        ++count;

        /* Cycle detection: if nxt has been seen before, we are in a cycle. */
        for (size_t i = 0; i < len; ++i) {
            if (visited[i] == nxt) {
                free(visited);
                return 0;
            }
        }

        /* Store nxt for future cycle checks.  Grow the array if necessary. */
        if (len == cap) {
            cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(visited, cap * sizeof(idx_t));
            if (!tmp) {                    /* allocation failure */
                free(visited);
                return 0;
            }
            visited = tmp;
        }
        visited[len++] = nxt;
        cur = nxt;
    }
}

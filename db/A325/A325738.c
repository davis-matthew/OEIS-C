/*
 * A325738  First term of n-th difference sequence of (floor(e*k/2)), k >= 0.
 * https://oeis.org/A325738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = the first term of the n‑th forward difference of f(k) =
 *            floor(e*k/2), k >= 0, with offset 1
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 38, i64 n <= 68, i128 n <= 132,
 *            big any n
 * Helpers:   floor_e_over_2_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_e_over_2_k.h"

/* Compute a(n) by successive forward differences.
   Complexity: O(n²) arithmetic operations, < 2 s for n ≤ 10 000. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of defined range, harmless */
    size_t sz = (size_t)n + 1;       /* number of terms of f(k) needed */

    /* Allocate buffer for current level values, size reduces with each level. */
    seq_t *buf = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!buf) abort();               /* memory failure */

    /* initialise buf with f(k) for k = 0 … n */
    for (size_t i = 0; i < sz; ++i)
        buf[i] = floor_e_over_2_k(i);

    /* successive forward differences in place */
    for (size_t level = 1; level <= n; ++level) {
        /* compute next level differences */
        for (size_t i = 0; i + 1 < sz; ++i)
            buf[i] = buf[i + 1] - buf[i];

        /* if this is the n‑th level, return the first element */
        if (level == n) {
            seq_t res = buf[0];
            free(buf);
            return res;
        }
        /* shrink working size for next level */
        sz--;
    }
    /* unreachable, but keep the compiler happy */
    free(buf);
    return 0;
}

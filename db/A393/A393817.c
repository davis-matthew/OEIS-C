/*
 * A393817  Return time for an n X n picture by the baker's map.
 * https://oeis.org/A393817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 21, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   baker_next, gcd, visited_get, visited_set
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A393817: Return time for an n×n picture by the baker's map.
 * The baker's map is a permutation on the set {0,…,n-1}².
 * a(n) is the least common multiple of the lengths of all cycles of this
 * permutation; equivalently, the smallest k such that baker^k(i,j) = (i,j)
 * for every (i,j).  This implementation iterates over all points,
 * finds each cycle with a simple loop and updates the running LCM. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/baker_next.h"
#include "../utils/visited_get.h"
#include "../utils/visited_set.h"

/* Return the exponent of the baker's map on an n×n grid. */
seq_t a(idx_t n) {
    if (n == 0) return 0;     /* safety: offset starts at 1 */

    idx_t total  = n * n;
    size_t bytes = (total + 7) >> 3;      /* visited bitmap */
    uint8_t *vis = (uint8_t *)calloc(bytes, 1);
    if (!vis) return 0;        /* allocation failure */

    seq_t ord = (seq_t)1;      /* current LCM of cycle lengths */

    for (idx_t start = 0; start < total; ++start) {
        if (visited_get(vis, start)) continue;

        /* Traverse the cycle starting from `start`. */
        idx_t cur = start;
        idx_t len = 0;
        do {
            visited_set(vis, cur);
            cur = baker_next(cur, n);
            ++len;
        } while (cur != start);

        seq_t cycle_len = (seq_t)len;
        seq_t g = gcd(ord, cycle_len);
        ord = (ord / g) * cycle_len;   /* safe for arbitrary-precision seq_t */
    }

    free(vis);
    return ord;
}

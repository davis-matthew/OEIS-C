/*
 * A261226  a(n) = number of steps to reach 0 when starting from k = n and
 *          repeatedly applying the map that replaces k with k - A055401(k),
 *          where A055401(k) = the number of positive cubes needed to sum to k
 *          using the greedy algorithm.
 * https://oeis.org/A261226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1799, u16 n <= 9403, u32 n <= 9403, u64 n <= 9403,
 *            u128 n <= 9403, big any n
 * Helpers:   cube_root, greedy_cube_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0;
   for n >= 1, a(n) = 1 + a(n - A055401(n)),
   where A055401(n) is the number of positive cubes needed
   to express n using the greedy algorithm. */

#include <stdint.h>
#include "../utils/cube_root.h"
#include "../utils/greedy_cube_count.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    seq_t steps = 0;
    while (n > 0) {
        ++steps;
        seq_t sub = greedy_cube_count(n);
        n -= (idx_t)sub;    /* sub <= n, cast is safe */
    }
    return steps;
}

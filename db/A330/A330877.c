/*
 * A330877  Number of steps needed to reach zero or a cycle when starting from
 *          k = n and repeatedly applying the map that replaces k by k - d(k)
 *          if k is even, by k + d(k) if k is odd, where d(k) is the number of
 *          divisors of k (A000005).
 * https://oeis.org/A330877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of mapping steps needed to reach 0 or the first
 *            step that enters a cycle when repeatedly applying k → k - d(k)
 *            if k is even, k → k + d(k) if k is odd (d(k) is the number of
 *            divisors of k)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

static inline uint64_t map_step(uint64_t k)
{
    if (k == 0) return 0;
    uint64_t d = divisor_count_u64(k);
    return (k & 1) ? k + d : k - d;
}

seq_t a(idx_t n)
{
    /* Treat n as a 64‑bit integer for the simulation. */
    uint64_t start = (uint64_t)n;

    if (start == 0)
        return (seq_t)0;

    /* First detect whether the orbit ends at 0 or falls into a cycle
       using Floyd's cycle‑finding algorithm. */
    uint64_t tort = map_step(start);
    uint64_t hare = map_step(map_step(start));

    while (tort != hare && hare != 0) {
        tort = map_step(tort);
        hare = map_step(map_step(hare));
    }

    if (hare == 0) {
        /* No cycle – count the steps until 0. */
        uint64_t steps = 0;
        uint64_t k = start;
        while (k != 0) {
            k = map_step(k);
            steps++;
        }
        return (seq_t)steps;
    }

    /* A cycle was found; compute the length μ of the pre‑cycle
       (the number of distinct values before the first step into the loop). */
    uint64_t mu = 0;
    tort = start;
    while (tort != hare) {
        tort = map_step(tort);
        hare = map_step(hare);
        mu++;
    }
    return (seq_t)mu;
}

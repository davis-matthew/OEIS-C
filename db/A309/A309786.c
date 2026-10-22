/*
 * A309786  a(n) is the length of the cycle of the trajectory of 1/n under the
 *          map f(x) = min(2*x, 2-2*x).
 * https://oeis.org/A309786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the cycle of the trajectory of 1/n under the
 *            map f(x) = min(2*x, 2-2*x). The implementation uses Floyd's
 *            cycle‑finding algorithm
 * Range:     u8 n <= 521, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

typedef struct { seq_t p, q; } state;

// Compute the next rational in the trajectory, reduced to lowest terms.
static inline state next_state(state s)
{
    seq_t np = s.p * (seq_t)2;
    seq_t nq = s.q;
    if (np > nq)
        np = (seq_t)2 * nq - np;
    seq_t g = gcd(np, nq);
    state r = { np / g, nq / g };
    return r;
}

// Return the cycle length of the map trajectory starting at 1/n.
seq_t a(idx_t n)
{
    state start = { (seq_t)1, (seq_t)n };

    // Floyd's cycle detection
    state tortoise = next_state(start);
    state hare = next_state(next_state(start));
    while (!(tortoise.p == hare.p && tortoise.q == hare.q))
    {
        tortoise = next_state(tortoise);
        hare = next_state(next_state(hare));
    }

    // Determine the length of the cycle
    seq_t len = 1;
    state cur = next_state(tortoise);
    while (!(cur.p == tortoise.p && cur.q == tortoise.q))
    {
        cur = next_state(cur);
        ++len;
    }
    return len;
}

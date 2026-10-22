/*
 * A353312  Size of the finite cycle eventually reached by iterating A353313,
 *          or -1 if no finite cycle is ever reached.
 * https://oeis.org/A353312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the finite cycle eventually reached by iterating
 *            A353313, or -1 if no finite cycle is ever reached
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9722, u64 n <= 9722,
 *            u128 n <= 9722, big any n
 * Helpers:   f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/f.h"
/* Definition of A353313 (used in the algorithm below):
 *   f(x) = x/3           if x is divisible by 3
 *          x + ((2*x + 7)/3)   otherwise             (integer division)
 *   The sequence a(n) is the length of the recurring cycle obtained by
 *   repeatedly applying f to the starting value n. */
 

/* Main function to compute a(n).  Floyd’s cycle‑finding algorithm
 * (tortoise and hare) is used so that only O(1) memory is required.
 * A hard limit on the number of steps is applied to detect the
 * impossible case of “no finite cycle” (returning (seq_t)-1). */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    seq_t tortoise = f(start);
    seq_t hare = f(tortoise);

    /* Maximum iterations to guard against non‑cycling or runaway sequences. */
    const idx_t MAX_STEPS = 2000000;
    idx_t steps = 0;

    /* Phase 1: Find a meeting point, if any. */
    while (tortoise != hare) {
        if (++steps > MAX_STEPS)
            return (seq_t)-1;   /* no cycle detected within the bound */
        tortoise = f(tortoise);
        hare = f(f(hare));
    }

    /* Phase 2: Find the entry point of the cycle. */
    seq_t ptr1 = start;
    seq_t ptr2 = tortoise;   /* same as hare at meeting point */
    while (ptr1 != ptr2) {
        ptr1 = f(ptr1);
        ptr2 = f(ptr2);
    }

    /* Phase 3: Compute the length of the cycle. */
    seq_t curr = f(ptr1);
    seq_t len = 1;
    while (curr != ptr1) {
        curr = f(curr);
        len++;
    }

    return len;
}

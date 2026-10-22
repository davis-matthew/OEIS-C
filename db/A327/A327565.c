/*
 * A327565  Number of transfers of marbles between two sets until the first
 *          repetition.
 * https://oeis.org/A327565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 904, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulation: compute a(n) by iterating the marble transfer
   process until the first repetition occurs.   */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but safe */
    /* Total number of marbles in both sets */
    seq_t total = (seq_t) n * ((seq_t)2);
    idx_t max_val = (idx_t) total;           /* maximum possible value of a */

    /* Allocate a visited array of size max_val+1 */
    size_t sz = (size_t)max_val + 1;
    bool *visited = (bool *) malloc(sz);
    if (!visited) return (seq_t)0;           /* memory failure */
    memset(visited, 0, sz);

    seq_t a_val = (seq_t) n;
    bool fromA = true;          /* next move: transfer from A to B */
    size_t steps = 0;

    visited[(size_t)a_val] = true;   /* initial state is visited */

    for (;;)
    {
        seq_t b_val = total - a_val;
        seq_t new_a;
        if (fromA)
        {
            /* Transfer ceil(a/2) from A to B: new_a = floor(a/2) */
            new_a = a_val >> 1;
        }
        else
        {
            /* Transfer ceil(b/2) from B to A: new_a = a + ceil(b/2) */
            seq_t y = (b_val + 1) >> 1;       /* ceil(b/2) */
            new_a = a_val + y;
        }
        ++steps;   /* we performed one move */

        if (visited[(size_t)new_a])
        {
            free(visited);
            return (seq_t)steps;
        }

        visited[(size_t)new_a] = true;
        a_val = new_a;
        fromA = !fromA;
    }
}

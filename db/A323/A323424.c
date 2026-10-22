/*
 * A323424  Number of cycles (mod n) under Collatz map.
 * https://oeis.org/A323424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   collatz_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A323424: number of directed cycles in the functional
   graph x → f(x) (mod n), where f(x) = x/2 if x is even, otherwise 3*x+1. */

#include <stdlib.h>
#include "../utils/collatz_mod.h"

seq_t a(idx_t n)
{
    unsigned char *state = (unsigned char *)malloc((size_t)n);
    if (!state)
        return 0;                 /* allocation failure */

    for (idx_t i = 0; i < n; ++i)
        state[i] = 0;              /* 0 = unvisited, 1 = visiting, 2 = processed */

    seq_t cycles = 0;

    for (idx_t start = 0; start < n; ++start) {
        if (state[start] != 0)
            continue;              /* already processed */

        /* Follow the path from start until we hit an already seen vertex. */
        idx_t v = start;
        while (state[v] == 0) {
            state[v] = 1;          /* mark as visiting */
            v = collatz_mod(v, n);
        }

        /* If we entered a vertex still marked as visiting, we found a new cycle. */
        if (state[v] == 1) {
            ++cycles;
            /* Mark all nodes in the cycle as processed. */
            idx_t w = v;
            do {
                state[w] = 2;
                w = collatz_mod(w, n);
            } while (w != v);
        }

        /* Mark the remaining path (outside the cycle) as processed. */
        v = start;
        while (state[v] == 1) {
            state[v] = 2;
            v = collatz_mod(v, n);
        }
    }

    free(state);
    return cycles;
}

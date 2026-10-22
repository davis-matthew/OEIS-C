/*
 * A352635  Number of cyclic orbits of the function f(x) = x^2 + 1 on Z/nZ.
 * https://oeis.org/A352635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cyclic orbits of the map x -> x^2 + 1 (mod n)
 *            on Z/nZ
 * Range:     u8 n <= 1767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* not defined, but return 0 just in case */
        return 0;

    static uint8_t *state = NULL;   /* 0 = unvisited, 1 = visiting, 2 = finished */
    static idx_t allocated = 0;     /* current size of state array */

    /* Resize the state array if necessary */
    if (allocated < n) {
        free(state);
        state = (uint8_t *)malloc(n * sizeof(uint8_t));
        allocated = n;
    }

    /* Reset the state array */
    memset(state, 0, n * sizeof(uint8_t));

    seq_t cycles = 0;
    seq_t mod = (seq_t)n;  /* modulus for the function */

    for (idx_t i = 0; i < n; ++i) {
        if (state[i] != 0)
            continue;

        /* First traversal to discover a cycle or reach an already visited node */
        idx_t x = i;
        while (state[x] == 0) {
            state[x] = 1;                         /* mark as on the current path */
            seq_t y = ((seq_t)x * x + 1) % mod;   /* next node */
            x = (idx_t)y;
        }

        /* If we looped back to a node still on this path, we found a cycle */
        if (state[x] == 1)
            ++cycles;

        /* Second traversal to mark all nodes in this component as finished */
        x = i;
        while (state[x] == 1) {
            state[x] = 2;                         /* finished */
            seq_t y = ((seq_t)x * x + 1) % mod;
            x = (idx_t)y;
        }
    }

    return cycles;
}

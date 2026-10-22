/*
 * A366494  a(n) is the number of cycles of the map f(x) = 10*x mod (10*n -
 *          1).
 * https://oeis.org/A366494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of cycles of f(x) = 10*x mod (10*n - 1), excluding
 *            the trivial cycle containing 0
 * Range:     u8 n <= 99, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used for OEIS offset */
    idx_t m = 10 * n - 1;                /* modulus of the map */

    /* Allocate state array: 0 = unvisited, 1 = in current walk,
       2 = finished.  In C++ a cast to unsigned char* is required. */
    unsigned char *state = (unsigned char *)malloc(m);
    if (!state) return 0;                /* allocation failure */

    for (idx_t i = 0; i < m; ++i) state[i] = 0;

    idx_t cycles = 0;

    for (idx_t i = 0; i < m; ++i) {
        if (state[i] != 0) continue;     /* already processed */

        idx_t cur = i;
        /* first walk: follow until a visited node is reached */
        while (state[cur] == 0) {
            state[cur] = 1;
            cur = (10 * cur) % m;
        }
        /* if we returned to a node still marked 1, a new cycle is found */
        if (state[cur] == 1) cycles++;

        /* second walk: mark the whole walk as finished */
        cur = i;
        while (state[cur] == 1) {
            state[cur] = 2;
            cur = (10 * cur) % m;
        }
    }

    free(state);

    /* The fixed point at 0 forms its own cycle; exclude it. */
    if (cycles > 0) cycles--;

    return (seq_t)cycles;
}

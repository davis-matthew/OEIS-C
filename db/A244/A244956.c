/*
 * A244956  Smallest positive multiple of n whose base-5 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive multiple of n whose base‑5
 *            representation uses only the digits 0 and 1. The function
 *            performs a BFS on the remainder graph modulo n. It is valid for
 *            any seq_t type (fixed or arbitrary‑precision)
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 283, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)           /* trivial case */
        return (seq_t)1;

    /* n is size_t here; it is guaranteed to fit (n ≤ 10 000 in tests). */
    const size_t N = (size_t)n;

    /* Allocate visited array and clear it */
    bool *visited = (bool *)calloc(N, sizeof(bool));
    if (!visited)
        return (seq_t)0;          /* allocation failure */

    /* Store the number corresponding to each remainder */
    seq_t *value = (seq_t *)calloc(N, sizeof(seq_t));
    if (!value) {
        free(visited);
        return (seq_t)0;
    }

    /* Queue for BFS (indices are remainders). */
    size_t *queue = (size_t *)malloc(N * sizeof(size_t));
    if (!queue) {
        free(visited);
        free(value);
        return (seq_t)0;
    }

    size_t head = 0, tail = 0;

    /* Start from the single digit “1” in base 5. */
    const size_t r0 = 1 % N;
    visited[r0] = true;
    value[r0]   = (seq_t)1;
    queue[tail++] = r0;

    while (head < tail) {
        const size_t r    = queue[head++];
        const seq_t cur   = value[r];

        /* Append a 0 or a 1 to the base‑5 representation. */
        for (int d = 0; d <= 1; ++d) {
            const seq_t new_val = cur * (seq_t)5 + (seq_t)d;
            const size_t r2 = (r * 5 + d) % N;

            if (!visited[r2]) {
                visited[r2] = true;
                value[r2]   = new_val;

                if (r2 == 0) {                      /* solution found */
                    seq_t result = new_val;
                    free(visited);
                    free(value);
                    free(queue);
                    return result;
                }

                queue[tail++] = r2;
            }
        }
    }

    /* The algorithm must always find a solution, but return 0 on failure. */
    free(visited);
    free(value);
    free(queue);
    return (seq_t)0;
}

/*
 * A327551  Number of cycles in the perfect shuffle permutation mapping
 *          (1,2,...,2n) to (1,n+1,2,n+2,...,n,2n).
 * https://oeis.org/A327551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2047, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of cycles in the perfect shuffle permutation
   mapping (1,2,...,2n) to (1,n+1,2,n+2,...,n,2n). */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Handle n < 1 just in case (undefined behaviour otherwise) */
    if (n < 1) return (seq_t)0;

    const idx_t N = 2 * n;                  /* total number of positions */
    bool *visited = (bool *)malloc((size_t)(N + 1) * sizeof(bool));
    if (!visited) return (seq_t)0;          /* allocation failure */

    memset(visited, 0, (size_t)(N + 1) * sizeof(bool));

    idx_t cycles = 0;

    for (idx_t i = 1; i <= N; ++i) {
        if (!visited[i]) {
            /* start a new cycle */
            idx_t j = i;
            do {
                visited[j] = true;
                /* permutation mapping: f(k) = 2*k-1 if k<=n else 2*(k-n) */
                if (j <= n)
                    j = 2 * j - 1;
                else
                    j = 2 * (j - n);
            } while (j != i);
            ++cycles;
        }
    }

    free(visited);
    return (seq_t)cycles;
}

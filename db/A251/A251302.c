/*
 * A251302  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (n+1)×3 arrays over {0,1,2,3} with no 2×2 subblock
 *            whose diagonal-sum is less than the maximum of its antidiagonal.
 *            Computation: dynamic programming on the 64 possible 3‑tuples of
 *            a row
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char adj[64][64] = {0};
static int adj_built = 0;

/* One‑time construction of the 64×64 adjacency matrix.
   adj[i][j] = 1 iff the 2×2 subblocks formed by rows i and j satisfy the
   constraint. */
static void build_adj(void)
{
    for (idx_t i = 0; i < 64; ++i) {
        unsigned a0 = i & 3;
        unsigned a1 = (i >> 2) & 3;
        unsigned a2 = (i >> 4) & 3;
        for (idx_t j = 0; j < 64; ++j) {
            unsigned b0 = j & 3;
            unsigned b1 = (j >> 2) & 3;
            unsigned b2 = (j >> 4) & 3;

            unsigned s01 = a0 + b1;          /* diagonal of first 2×2 block   */
            unsigned s12 = a1 + b2;          /* diagonal of second 2×2 block  */
            unsigned m01 = (a1 > b0) ? a1 : b0; /* max of antidiagonal of first block */
            unsigned m12 = (a2 > b1) ? a2 : b1; /* max of antidiagonal of second block */

            adj[i][j] = (s01 >= m01 && s12 >= m12) ? 1 : 0;
        }
    }
    adj_built = 1;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* the offset starts at n = 1 */

    if (!adj_built) build_adj();          /* lazy construction */

    seq_t cur[64], next[64];
    /* start with a single row (any of the 64 possibilities) */
    for (idx_t i = 0; i < 64; ++i) cur[i] = (seq_t)1;

    /* step = number of rows processed minus 1.
       After step steps we have arrays of size (step+1)+1 = step+2 rows.
       For step = n we obtain arrays of size (n+1)×3, i.e. the required a(n). */
    for (idx_t step = 1; step <= n; ++step) {
        memset(next, 0, sizeof(next));

        for (idx_t i = 0; i < 64; ++i) {
            if (cur[i] == (seq_t)0) continue;
            for (idx_t j = 0; j < 64; ++j) {
                if (adj[i][j]) next[j] += cur[i];
            }
        }

        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < 64; ++j) sum += next[j];

        if (step == n) return sum;        /* a(n) found */

        memcpy(cur, next, sizeof(cur));
    }

    /* should never reach here */
    return (seq_t)0;
}

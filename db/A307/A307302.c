/*
 * A307302  Array read by antidiagonals: Sprague-Grundy values for the game
 *          NimHof with rules [1,0], [3,3], [0,1].
 * https://oeis.org/A307302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the Sprague–Grundy value of NimHof with rules [1,0],
 *            [3,3], [0,1] read row‑by‑row along antidiagonals. For
 *            coordinates (x,y) with x+y=s, the SG value is defined by SG(x,y)
 *            = mex { SG(x-i,y) (1≤i≤x) , SG(x,y-i) (1≤i≤y) , SG(x-3k,y-3k)
 *            (1≤k≤min(x,y)/3) }; a(n) = SG(x,y) where (x,y) are the...
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the Sprague–Grundy value as described. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal level 's' containing index n. */
    idx_t target = n + 1;
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 < target)
        ++s;

    /* Convert the linear index to 2‑D coordinates (x,y). */
    idx_t offset = n - s * (s + 1) / 2;
    idx_t x = s - offset;
    idx_t y = offset;

    /* Allocate a square table large enough for all positions with sum ≤ s. */
    idx_t dim = s + 1;
    size_t table_size = (size_t)dim * (size_t)dim;
    seq_t *sg = (seq_t *)calloc(table_size, sizeof(seq_t));
    if (!sg)
        return 0;                       /* allocation failure → 0 */

    /* Base position (0,0) has SG value 0. */
    sg[0] = 0;

    /* Fill the table in order of increasing sums. */
    for (idx_t sum = 1; sum <= s; ++sum) {
        /* Enumerate x from sum down to 0 (descending order) to match
         * the antidiagonal ordering used in the OEIS definition. */
        for (int ix = (int)sum; ix >= 0; --ix) {
            idx_t xv = (idx_t)ix;
            idx_t yv = sum - xv;

            /* Collect SG values of all reachable positions. */
            seq_t reachable[512];
            size_t rc = 0;

            /* Moves that subtract from the first heap. */
            for (idx_t i = 1; i <= xv; ++i)
                reachable[rc++] = sg[(xv - i) * dim + yv];

            /* Moves that subtract from the second heap. */
            for (idx_t i = 1; i <= yv; ++i)
                reachable[rc++] = sg[xv * dim + (yv - i)];

            /* Moves that subtract equal multiples of 3 from both heaps. */
            idx_t min3 = xv < yv ? xv : yv;
            min3 = min3 / 3;
            for (idx_t k = 1; k <= min3; ++k)
                reachable[rc++] = sg[(xv - 3 * k) * dim + (yv - 3 * k)];

            /* Compute mex of the reachable SG values. */
            seq_t mex = 0;
            while (true) {
                bool found = false;
                for (size_t j = 0; j < rc; ++j)
                    if (reachable[j] == mex) {
                        found = true;
                        break;
                    }
                if (!found)
                    break;
                ++mex;
            }

            sg[xv * dim + yv] = mex;
        }
    }

    /* Retrieve the result for the requested index. */
    seq_t result = sg[x * dim + y];
    free(sg);
    return result;
}

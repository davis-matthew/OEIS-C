/*
 * A246925  8 X 8 square array read by rows: T(i,j) = number of moves of a
 *          knight to reach the square (i,j) when starting from the corner
 *          square (1,1).
 * https://oeis.org/A246925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, full, tabf, fini
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum knight moves from corner (1,1) to each square on an 8×8 board,
   flattened row‑major order.  The values are precomputed once using a
   breadth‑first search.  For indices n > 64 the sequence is defined to
   be 0. */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for the 8×8 distance table. */
    static bool computed = false;
    static seq_t dist[64];

    if (!computed) {
        const int SIZE = 8;
        const int N   = SIZE * SIZE;          /* 64 */
        const seq_t INF = (seq_t)-1;          /* sentinel for unvisited cells */

        /* Mark all cells as unvisited. */
        for (size_t i = 0; i < N; ++i)
            dist[i] = INF;

        /* Knight move deltas. */
        const int dr[8] = { 1, 1, 2, 2, -1, -1, -2, -2 };
        const int dc[8] = { 2, -2, 1, -1, 2, -2, 1, -1 };

        /* BFS queue implemented as a simple circular buffer. */
        size_t queue[64];
        size_t head = 0, tail = 0;

        /* Start from corner (0,0). */
        dist[0] = (seq_t)0;
        queue[tail++] = 0;

        while (head < tail) {
            size_t idx   = queue[head++];
            int r = (int)(idx / SIZE);
            int c = (int)(idx % SIZE);
            seq_t d = dist[idx];

            for (int k = 0; k < 8; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];
                if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) {
                    size_t nidx = (size_t)(nr * SIZE + nc);
                    if (dist[nidx] == INF) {
                        dist[nidx] = d + (seq_t)1;
                        queue[tail++] = nidx;
                    }
                }
            }
        }

        computed = true;
    }

    /* Return the requested value if within bounds. */
    if (n > 0 && n <= 64)
        return dist[(size_t)(n - 1)];
    return (seq_t)0;
}

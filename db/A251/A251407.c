/*
 * A251407  Number of (n+1)X(5+1) 0..2 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 0..2 arrays such that each 2×2 subblock
 *            sums to a prime
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static data structures, initialized on the first call */
    static bool ready = false;
    static int rows[729][6];
    static int neigh_count[729];
    static int neighbors[729][729];

    if (!ready)
    {
        /* Precompute the decimal representation of each row (base‑3). */
        for (int id = 0; id < 729; id++)
        {
            int tmp = id;
            for (int pos = 5; pos >= 0; pos--)
            {
                rows[id][pos] = tmp % 3;
                tmp /= 3;
            }
        }

        /* Build the adjacency lists: pattern p → pattern q is allowed
           iff every 2×2 block formed by rows p and q sums to a prime
           (2, 3, 5 or 7). */
        for (int p = 0; p < 729; p++)
        {
            neigh_count[p] = 0;
            for (int q = 0; q < 729; q++)
            {
                bool ok = true;
                for (int j = 0; j < 5; j++)
                {
                    int s = rows[p][j] + rows[p][j + 1] +
                            rows[q][j] + rows[q][j + 1];
                    if (!(s == 2 || s == 3 || s == 5 || s == 7))
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                    neighbors[p][neigh_count[p]++] = q;
            }
        }
        ready = true;
    }

    /* Two vectors for the dynamic programming. */
    static seq_t cur[729], nxt[729];
    for (int i = 0; i < 729; i++)
        cur[i] = 1;            /* each single row can start */

    /* Perform n transitions – the length becomes n+1 rows. */
    for (idx_t step = 0; step < n; step++)
    {
        for (int i = 0; i < 729; i++)
            nxt[i] = 0;

        for (int p = 0; p < 729; p++)
        {
            seq_t c = cur[p];
            if (c == 0)
                continue;
            int cnt = neigh_count[p];
            for (int k = 0; k < cnt; k++)
            {
                int q = neighbors[p][k];
                nxt[q] += c;
            }
        }
        /* swap vectors for the next iteration */
        for (int i = 0; i < 729; i++)
            cur[i] = nxt[i];
    }

    /* Sum over all ending rows. */
    seq_t total = 0;
    for (int i = 0; i < 729; i++)
        total += cur[i];

    return total;
}

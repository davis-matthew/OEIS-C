/*
 * A251454  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to a prime.
 * https://oeis.org/A251454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays with entries 0..3 such that
 *            every 2×2 subblock sums to a prime. The value is computed by
 *            dynamic programming with a transfer-matrix that records
 *            compatibility of consecutive rows
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Counting function ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Prime numbers up to 12 */
    static const bool is_prime[13] = {
        false, false, true, true, false, true, false, true, false, false, false, true, false
    };

    /* Pre‑computed adjacency: for each row i, store all rows j that can follow i. */
    static bool built = false;
    static int  adj[256][256];          /* adjacency lists */
    static unsigned char deg[256];      /* list lengths */

    if (!built)
    {
        /* Build adjacency lists */
        for (int i = 0; i < 256; ++i)
        {
            int di[4];          /* digits of row i */
            int x = i;
            for (int c = 0; c < 4; ++c) { di[c] = x & 3; x >>= 2; }

            int cur_deg = 0;
            for (int j = 0; j < 256; ++j)
            {
                int dj[4];      /* digits of row j */
                int y = j;
                for (int c = 0; c < 4; ++c) { dj[c] = y & 3; y >>= 2; }

                bool ok = true;
                for (int k = 0; k < 3; ++k)
                {
                    int s = di[k] + di[k + 1] + dj[k] + dj[k + 1];
                    if (!is_prime[s]) { ok = false; break; }
                }
                if (ok) adj[i][cur_deg++] = j;
            }
            deg[i] = (unsigned char)cur_deg;
        }
        built = true;
    }

    /* DP over rows -------------------------------------------------------- */
    static seq_t dp1[256];
    static seq_t dp2[256];
    seq_t *curr = dp1;
    seq_t *next = dp2;

    for (int i = 0; i < 256; ++i) curr[i] = 1;   /* first row can be anything */

    for (idx_t step = 1; step <= n; ++step)
    {
        /* reset next buffer */
        for (int i = 0; i < 256; ++i) next[i] = 0;

        /* transition step */
        for (int i = 0; i < 256; ++i)
        {
            seq_t val = curr[i];
            if (val == 0) continue;               /* skip zero transitions */
            unsigned char len = deg[i];
            for (unsigned char k = 0; k < len; ++k)
            {
                int j = adj[i][k];
                next[j] += val;
            }
        }
        /* swap buffers */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    /* Sum over all final rows --------------------------------------------- */
    seq_t result = 0;
    for (int i = 0; i < 256; ++i) result += curr[i];
    return result;
}

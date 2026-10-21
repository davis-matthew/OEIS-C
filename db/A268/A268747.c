/*
 * A268747  Number of nX5 binary arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n×5 binary arrays with at most one adjacency of two 1‑cells.
   The state tracks whether an adjacency has already occurred.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; no queries for n==0 are expected */

    /* horizontal pairs in all 32 patterns (LSB = column 0) */
    static const unsigned int hcnt[32] = {
        0,0,0,1,0,0,1,2,0,0,0,1,1,1,2,3,
        0,0,0,1,0,0,1,2,1,1,1,2,2,2,3,4
    };

    /* indices of patterns with at most one horizontal adjacency pair */
    static const unsigned int allowed[23] = {
        0,1,2,3,4,5,6,8,9,10,11,12,13,
        16,17,18,19,20,21,22,24,25,26
    };

    /* DP state: prev[p][used] = number of ways for the current top rows. */
    seq_t prev[32][2] = {{0}};
    seq_t cur[32][2];

    /* initialise for the first row */
    for (int i = 0; i < 23; ++i) {
        unsigned int s = allowed[i];
        int used = (hcnt[s] == 1) ? 1 : 0;
        prev[s][used] = (seq_t)1;
    }

    /* if only one row, sum all possibilities */
    if (n == 1) {
        seq_t total = 0;
        for (int i = 0; i < 23; ++i) {
            unsigned int s = allowed[i];
            total += prev[s][0];
            total += prev[s][1];
        }
        return total;
    }

    /* process subsequent rows */
    for (idx_t r = 1; r < n; ++r) {
        /* clear current layer */
        for (int i = 0; i < 32; ++i) cur[i][0] = cur[i][1] = 0;

        /* transition from previous row patterns to current row patterns */
        for (int ip = 0; ip < 23; ++ip) {
            unsigned int p = allowed[ip];
            for (int u = 0; u <= 1; ++u) {
                seq_t ways = prev[p][u];
                if (!ways) continue;

                for (int iq = 0; iq < 23; ++iq) {
                    unsigned int q = allowed[iq];
                    int qh = hcnt[q];                               /* horizontal in new row */
                    int vcnt = __builtin_popcount((unsigned int)(p & q));   /* vertical */

                    if (vcnt > 1) continue;                         /* more than one vertical pair */

                    int new_used = u;
                    if (qh == 1) {
                        if (new_used) continue;                     /* would create second pair */
                        new_used = 1;
                    }
                    if (vcnt == 1) {
                        if (new_used) continue;
                        new_used = 1;
                    }

                    cur[q][new_used] += ways;
                }
            }
        }

        /* copy current layer to previous for next iteration */
        for (int i = 0; i < 23; ++i) {
            unsigned int s = allowed[i];
            prev[s][0] = cur[s][0];
            prev[s][1] = cur[s][1];
        }
    }

    /* sum total ways over all possible last row patterns */
    seq_t total = 0;
    for (int i = 0; i < 23; ++i) {
        unsigned int s = allowed[i];
        total += prev[s][0];
        total += prev[s][1];
    }
    return total;
}

/*
 * A269030  Number of n X 3 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A269030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A269030 by dynamic programming over 27 row patterns
   (each 3‑cell row over {0,1,2}).  The state keeps track of whether the
   array built so far has seen 0 or exactly 1 sum‑2 adjacent pair.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for offset 1 */

    /* Build all 27 row patterns and auxiliary tables. */
    unsigned char row[27][3];
    int h2[27];                    /* horizontal sum‑2 edges inside a row */
    int cross[27][27];             /* sum‑2 edges between consecutive rows */

    for (int p = 0; p < 27; ++p) {
        int x = p;                           /* 0..26 representing 3 base‑3 digits */
        row[p][0] = x / 9;                   /* first cell */
        row[p][1] = (x / 3) % 3;             /* second cell */
        row[p][2] = x % 3;                   /* third cell */

        int s = 0;
        if (row[p][0] + row[p][1] == 2) ++s; /* horizontal left‑right */
        if (row[p][1] + row[p][2] == 2) ++s; /* horizontal right‑right */
        h2[p] = s;
    }

    for (int p = 0; p < 27; ++p)
        for (int q = 0; q < 27; ++q) {
            int c = 0;
            /* diagonal and anti‑diagonal between rows p (above) and q (below) */
            if (row[p][0] + row[q][1] == 2) ++c;
            if (row[p][1] + row[q][0] == 2) ++c;
            if (row[p][1] + row[q][2] == 2) ++c;
            if (row[p][2] + row[q][1] == 2) ++c;
            cross[p][q] = c;
        }

    /* DP arrays: dp0[p] = arrays ending with pattern p with 0 sum‑2 edges so far
                    dp1[p] = arrays ending with pattern p with exactly 1 sum‑2 edge so far. */
    seq_t dp0[27] = {0};
    seq_t dp1[27] = {0};
    seq_t newdp0[27] = {0};
    seq_t newdp1[27] = {0};

    /* First row initialization */
    for (int p = 0; p < 27; ++p) {
        int s = h2[p];
        if (s == 0)
            dp0[p] = 1;
        else if (s == 1)
            dp1[p] = 1;
        /* patterns with s==2 cannot lead to exactly one adjacency total */
    }

    /* Process subsequent rows */
    for (idx_t r = 2; r <= n; ++r) {
        /* reset next‑state arrays */
        for (int i = 0; i < 27; ++i) {
            newdp0[i] = 0;
            newdp1[i] = 0;
        }

        /* Transition from state with 0 sum‑2 edges */
        for (int p = 0; p < 27; ++p) {
            seq_t val0 = dp0[p];
            if (!val0) continue;
            for (int q = 0; q < 27; ++q) {
                int add = h2[q] + cross[p][q];
                if (add == 0)
                    newdp0[q] += val0;
                else if (add == 1)
                    newdp1[q] += val0;
                /* add > 1 would exceed the limit of exactly one adjacency; discard */
            }
        }

        /* Transition from state with exactly 1 sum‑2 edge */
        for (int p = 0; p < 27; ++p) {
            seq_t val1 = dp1[p];
            if (!val1) continue;
            for (int q = 0; q < 27; ++q) {
                /* Only allowed if adding the new row does NOT create a new adjacency */
                if (cross[p][q] == 0 && h2[q] == 0)
                    newdp1[q] += val1;
                /* otherwise total would exceed 1 and the array is discarded */
            }
        }

        /* Copy newdp into dp for next iteration */
        for (int i = 0; i < 27; ++i) {
            dp0[i] = newdp0[i];
            dp1[i] = newdp1[i];
        }
    }

    /* Sum all states with exactly one adjacency pair. */
    seq_t result = 0;
    for (int p = 0; p < 27; ++p)
        result += dp1[p];

    return result;
}

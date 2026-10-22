/*
 * A326509  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly eight 0's.
 * https://oeis.org/A326509
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 126, u64 n <= 1934,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of binary max‑heaps on n elements from {0,1}
 * that contain exactly eight 0's.  We compute it by dynamic programming
 * over the heap indices.  For a node u we store ways[u][k] = number of
 * ways to fill the subtree rooted at u with exactly k zeros.  At the
 * root ways[0][8] is the desired value.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;   /* the offset is 8 */

    /* Arrays for subtree sizes and DP table */
    idx_t *size = (idx_t *)malloc(n * sizeof(idx_t));
    seq_t *ways = (seq_t *)malloc(n * 9 * sizeof(seq_t));  /* k = 0..8 */

    if (!size || !ways) { /* allocation failed */
        free(size);
        free(ways);
        return 0;
    }

    /* Bottom‑up computation over the heap indices */
    for (idx_t i = n; i-- > 0; ) {
        idx_t left  = (i << 1) + 1;
        idx_t right = (i << 1) + 2;

        /* subtree size */
        idx_t sz = 1;
        if (left  < n) sz += size[left];
        if (right < n) sz += size[right];
        size[i] = sz;

        /* initialise ways[i][*] to zero */
        seq_t *row = ways + i * 9;
        for (int k = 0; k < 9; ++k)
            row[k] = 0;

        /* node equals 0: all descendants must be 0 */
        if (sz <= 8)
            row[(int)sz] += (seq_t)1;

        /* node equals 1: combine children */
        int maxL = (left  < n) ? (size[left]  > 8 ? 8 : (int)size[left])  : 0;
        int maxR = (right < n) ? (size[right] > 8 ? 8 : (int)size[right]) : 0;

        for (int iL = 0; iL <= maxL; ++iL) {
            seq_t valL = 0;
            if (left < n)
                valL = ways[left * 9 + iL];
            else if (iL == 0)
                valL = (seq_t)1;   /* missing child is an empty subtree */
            if (valL == 0) continue;

            for (int iR = 0; iR <= maxR; ++iR) {
                seq_t valR = 0;
                if (right < n)
                    valR = ways[right * 9 + iR];
                else if (iR == 0)
                    valR = (seq_t)1;
                if (valR == 0) continue;

                int z = iL + iR;
                if (z <= 8)
                    row[z] += valL * valR;
            }
        }
    }

    seq_t ans = ways[8];  /* row 0, column 8 */

    free(size);
    free(ways);
    return ans;
}

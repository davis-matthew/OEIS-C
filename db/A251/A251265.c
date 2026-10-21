/*
 * A251265  Number of (n+1)X(5+1) 0..1 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A251265: Count of (n+1)×6 0/1 arrays with no 2×2 subblock
 *  having x11−x00 < x10−x01.
 *
 *  The count is obtained by dynamic programming over rows.
 *  Each row is a 6‑bit pattern (0…63).  For two consecutive rows
 *  r1 (upper) and r2 (lower) the following must hold for every
 *  adjacent column pair k,k+1 (k=0…4):
 *
 *      (r2>>k+1 &1) + (r1>>k+1 &1)   >=
 *      (r2>>k   &1) + (r1>>k   &1)
 *
 *  This condition is equivalent to the required inequality for the
 *  2×2 block.  We precompute a 64×64 boolean table of allowed
 *  transitions between row patterns.  Starting with any of the 64
 *  patterns as the first row, we apply the transition n times
 *  (thus constructing n+1 rows) and sum the numbers of ways that
 *  end in each pattern.  The result is the desired a(n).  The
 *  algorithm is O(n·64²), comfortably fast for n≈10⁴.
 *
 *  The function works for any unsigned integer type `seq_t`
 *  provided by the build; all arithmetic uses that type.
 *  The index type `idx_t` is assumed unsigned.
 */

#include <stdbool.h>

#define STATE_CNT 64

seq_t a(idx_t n)
{
    /* Build the transition table once */
    static bool adj[STATE_CNT][STATE_CNT];
    static bool initialized = false;
    if (!initialized) {
        for (int r1 = 0; r1 < STATE_CNT; ++r1) {
            for (int r2 = 0; r2 < STATE_CNT; ++r2) {
                bool ok = true;
                for (int k = 0; k < 5; ++k) {
                    int a00 = (r1 >> k)   & 1;
                    int a01 = (r1 >> (k+1)) & 1;
                    int a10 = (r2 >> k)   & 1;
                    int a11 = (r2 >> (k+1)) & 1;
                    if ( (a11 + a01) < (a10 + a00) ) {
                        ok = false;
                        break;
                    }
                }
                adj[r1][r2] = ok;
            }
        }
        initialized = true;
    }

    /* Dynamic programming over rows */
    seq_t cur[STATE_CNT];
    seq_t nxt[STATE_CNT];
    for (int i = 0; i < STATE_CNT; ++i)
        cur[i] = 1;          /* any pattern can start the first row    */

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATE_CNT; ++i)
            nxt[i] = 0;

        for (int r1 = 0; r1 < STATE_CNT; ++r1) {
            seq_t ways = cur[r1];
            if (ways == 0) continue;
            for (int r2 = 0; r2 < STATE_CNT; ++r2) {
                if (adj[r1][r2])
                    nxt[r2] += ways;
            }
        }
        for (int i = 0; i < STATE_CNT; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all ways of ending in any final row pattern          */
    seq_t total = 0;
    for (int i = 0; i < STATE_CNT; ++i)
        total += cur[i];

    return total;
}

/*
 * A296670  Number of nX4 0..1 arrays with each 1 adjacent to 0, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/*  Recurrence: the number of n×4 0/1 arrays in which each 1 has 0, 2 or 3 king‑move
    neighboring 1s is obtained by a dynamic programme over rows.  The state
    consists of the two preceding rows (as 4‑bit patterns); the transition
    checks that the middle row satisfies the neighbour condition with its
    upper and lower neighbours.  The final row is required to be adjacent
    to a row of all zeros below. */

seq_t a(idx_t n)
{
    /* constants for 4‑bit patterns */
    #define M 16
    #define SZ 256   /* M*M */

    /* table of admissible triples (up,mid,down) */
    static bool val[M][M][M];
    static bool ready = false;
    if (!ready) {
        for (int up = 0; up < M; ++up) {
            for (int mid = 0; mid < M; ++mid) {
                for (int down = 0; down < M; ++down) {
                    bool ok = true;
                    for (int col = 0; col < 4 && ok; ++col) {
                        if ((mid >> col) & 1) {
                            int adj = 0;
                            /* same row neighbours */
                            if (col > 0 && (mid >> (col - 1)) & 1) ++adj;
                            if (col < 3 && (mid >> (col + 1)) & 1) ++adj;
                            /* upper row neighbours */
                            if (up >> col & 1) ++adj;
                            if (col > 0 && (up >> (col - 1)) & 1) ++adj;
                            if (col < 3 && (up >> (col + 1)) & 1) ++adj;
                            /* lower row neighbours */
                            if (down >> col & 1) ++adj;
                            if (col > 0 && (down >> (col - 1)) & 1) ++adj;
                            if (col < 3 && (down >> (col + 1)) & 1) ++adj;
                            if (!(adj == 0 || adj == 2 || adj == 3))
                                ok = false;
                        }
                    }
                    val[up][mid][down] = ok;
                }
            }
        }
        ready = true;
    }

    seq_t dp_curr[SZ];
    seq_t dp_next[SZ];

    /* initialise all DP entries to zero */
    for (int i = 0; i < SZ; ++i) {
        dp_curr[i] = 0;
        dp_next[i] = 0;
    }

    /* initial state: first row is a pattern, row above is all zeros */
    for (int pat = 0; pat < M; ++pat)
        dp_curr[pat] = (seq_t)1;      /* index 0*M + pat */

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero dp_next for the new iteration */
        for (int i = 0; i < SZ; ++i)
            dp_next[i] = 0;

        for (int prev2 = 0; prev2 < M; ++prev2)
            for (int prev1 = 0; prev1 < M; ++prev1) {
                seq_t cnt = dp_curr[prev2 * M + prev1];
                if (cnt == 0) continue;
                for (int newp = 0; newp < M; ++newp)
                    if (val[prev2][prev1][newp])
                        dp_next[prev1 * M + newp] += cnt;
            }

        /* copy next to current for the next iteration */
        for (int i = 0; i < SZ; ++i)
            dp_curr[i] = dp_next[i];
    }

    /* final row must be adjacent to a zero row below */
    seq_t total = 0;
    for (int prev2 = 0; prev2 < M; ++prev2)
        for (int prev1 = 0; prev1 < M; ++prev1) {
            seq_t cnt = dp_curr[prev2 * M + prev1];
            if (cnt == 0) continue;
            if (val[prev2][prev1][0])   /* bottom row is zero */
                total += cnt;
        }

    return total;
}

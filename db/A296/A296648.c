/*
 * A296648  Number of nX5 0..1 arrays with each 1 adjacent to 0, 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296648: Number of n×5 binary arrays with each 1 adjacent to 0, 1 or 3 king‐move neighbors.
   Computation by dynamic programming.
   The recurrence is implicit in the transition table of triples of consecutive rows.
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Pre‑compute which triples (a,b,c) of 5‑bit rows satisfy the adjacency rule
   for the middle row b when surrounded by rows a (above) and c (below). */
static bool triple_valid[1 << 5][1 << 5][1 << 5];
static bool valid_init = false;

static void
init_triple_valid(void)
{
    if (valid_init) return;
    for (int a = 0; a < (1 << 5); ++a) {
        for (int b = 0; b < (1 << 5); ++b) {
            for (int c = 0; c < (1 << 5); ++c) {
                bool ok = true;
                for (int j = 0; j < 5 && ok; ++j) {
                    if ((b >> j) & 1) {
                        int cnt = 0;
                        /* horizontal neighbours in the same row */
                        if (j > 0 && ((b >> (j - 1)) & 1)) ++cnt;
                        if (j < 4 && ((b >> (j + 1)) & 1)) ++cnt;
                        /* neighbours from the row above a */
                        if ((a >> j) & 1) ++cnt;
                        if (j > 0 && ((a >> (j - 1)) & 1)) ++cnt;
                        if (j < 4 && ((a >> (j + 1)) & 1)) ++cnt;
                        /* neighbours from the row below c */
                        if ((c >> j) & 1) ++cnt;
                        if (j > 0 && ((c >> (j - 1)) & 1)) ++cnt;
                        if (j < 4 && ((c >> (j + 1)) & 1)) ++cnt;
                        if (!(cnt == 0 || cnt == 1 || cnt == 3))
                            ok = false;
                    }
                }
                triple_valid[a][b][c] = ok;
            }
        }
    }
    valid_init = true;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Ensure the transition table is ready. */
    init_triple_valid();

    /* dp_prev[a][b] – number of ways to build rows up to the current row
       where row a is the preceding row and row b is the current (last real) row.
       dp_next is used for the next iteration. */
    seq_t dp_prev[1 << 5][1 << 5];
    seq_t dp_next[1 << 5][1 << 5];

    /* Initialise all entries to zero.  The assignment works both for native
       integer types and for C++ arbitrary‑precision integer classes. */
    for (int i = 0; i < (1 << 5); ++i)
        for (int j = 0; j < (1 << 5); ++j)
            dp_prev[i][j] = 0;

    /* First real row: preceding row is all zeros.  Every b is initially
       counted as one possibility – validity will be checked against the
       transition to the next row. */
    for (int b = 0; b < (1 << 5); ++b)
        dp_prev[0][b] = 1;

    /* Iterate over the remaining rows (if any). */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dp_next. */
        for (int i = 0; i < (1 << 5); ++i)
            for (int j = 0; j < (1 << 5); ++j)
                dp_next[i][j] = 0;

        for (int a = 0; a < (1 << 5); ++a) {
            for (int b = 0; b < (1 << 5); ++b) {
                seq_t cnt = dp_prev[a][b];
                if (!cnt) continue;
                for (int c = 0; c < (1 << 5); ++c) {
                    if (triple_valid[a][b][c])
                        dp_next[b][c] += cnt;
                }
            }
        }
        /* Swap dp_prev and dp_next for the next iteration. */
        for (int i = 0; i < (1 << 5); ++i)
            for (int j = 0; j < (1 << 5); ++j)
                dp_prev[i][j] = dp_next[i][j];
    }

    /* Add the terminating zero row to check the last real row. */
    seq_t result = 0;
    for (int a = 0; a < (1 << 5); ++a) {
        for (int b = 0; b < (1 << 5); ++b) {
            seq_t cnt = dp_prev[a][b];
            if (!cnt) continue;
            if (triple_valid[a][b][0])         /* trailing zero row */
                result += cnt;
        }
    }
    return result;
}

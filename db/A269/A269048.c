/*
 * A269048  Number of n X 4 0..2 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269048: number of n×4 arrays with entries 0..2 in which at most
   one horizontally, diagonally or anti‑diagonally adjacent pair has sum 2.
   This implementation uses dynamic programming over all 3^4 = 81 possible
   row patterns.  For each pattern we store the number of internal
   adjacent–sum‑2 pairs (0 or 1).  Between two consecutive rows we pre‑
   compute the number of cross‑row adjacent‑sum‑2 pairs (0,1, or >1;
   >1 is rejected).  The DP keeps track of whether a sum‑2 pair has
   already been used (state 0 or 1).  Transitions that would cause
   more than one such pair are discarded.  The algorithm runs in
   O(n·81²), which is well under the limits for n up to 10 000. */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Helper arrays, initialized once */
static bool _init_done = false;
static char _cells[81][4];
static unsigned char _internal[81];          /* 0 or 1 */
static bool _valid[81];
static signed char _cross[81][81];          /* -1 = invalid, 0 or 1 otherwise */

static void _init(void)
{
    int pat = 0;
    /* enumerate all 3^4 patterns */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d, ++pat) {
                    _cells[pat][0] = a;
                    _cells[pat][1] = b;
                    _cells[pat][2] = c;
                    _cells[pat][3] = d;
                    int inc = 0;
                    for (int k = 0; k < 3; ++k)
                        if (_cells[pat][k] + _cells[pat][k + 1] == 2)
                            inc++;
                    _internal[pat] = inc;
                    _valid[pat]     = (inc <= 1);
                }
    /* pre‑compute cross‑row counts */
    for (int p = 0; p < 81; ++p)
        for (int q = 0; q < 81; ++q) {
            if (!_valid[p] || !_valid[q]) {
                _cross[p][q] = -1;
                continue;
            }
            int cc = 0;
            for (int k = 0; k < 3; ++k) {
                if (_cells[p][k] + _cells[q][k + 1] == 2) cc++;
                if (_cells[p][k + 1] + _cells[q][k] == 2) cc++;
            }
            _cross[p][q] = (cc > 1 ? -1 : (signed char)cc);
        }
    _init_done = true;
}

seq_t a(idx_t n)
{
    /* Ensure tables are ready */
    if (!_init_done)
        _init();

    /* dp[pattern][used] : used = 0 or 1 */
    static seq_t dp[81][2];
    static seq_t next[81][2];
    /* initialise for first row */
    memset(dp, 0, sizeof(dp));
    for (int p = 0; p < 81; ++p) {
        if (_valid[p])
            dp[p][_internal[p]] = 1;
    }

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        memset(next, 0, sizeof(next));
        for (int p = 0; p < 81; ++p) if (_valid[p]) {
            int inc_p = _internal[p];
            for (int q = 0; q < 81; ++q) {
                signed char cc = _cross[q][p];
                if (cc < 0) continue;          /* invalid transition */
                for (int used = 0; used <= 1; ++used) {
                    if (dp[q][used] == 0) continue;
                    int new_used = used + inc_p + cc;
                    if (new_used <= 1)
                        next[p][new_used] += dp[q][used];
                }
            }
        }
        /* copy next into dp for next iteration */
        for (int p = 0; p < 81; ++p) {
            dp[p][0] = next[p][0];
            dp[p][1] = next[p][1];
        }
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (int p = 0; p < 81; ++p)
        total += dp[p][0] + dp[p][1];

    return total;
}

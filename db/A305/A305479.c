/*
 * A305479  Number of n X 5 0..1 arrays with every element unequal to 0, 1, 3,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence‑based DP for A305479 Number of n × 5 0..1 arrays with
 *            every element unequal to 0, 1, 3, 6 or 8 king‑move adjacent
 *            elements, upper‑left fixed to 0. The allowed count of *unequal*
 *            king‑move neighbours for each cell must be one of {0,1,3,6,8}.
 *            An O(n) dynamic programming with a state of t...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 53, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Check whether a row 'curr' is valid given the rows 'up' and 'down'.
   'up' or 'down' may be -1 to indicate a non‑existing row (border). */
static inline int is_valid_row(int up, int curr, int down)
{
    /* pre‑extract bits of the three rows for speed */
    int above[5], curb[5], below[5];
    for (int col = 0; col < 5; ++col) {
        curb[col] = (curr >> col) & 1;
        above[col] = (up  >= 0) ? ((up  >> col) & 1) : -1;
        below[col] = (down >= 0) ? ((down >> col) & 1) : -1;
    }

    /* Allowed numbers of unequal neighbours */
    const int allowed[5] = {0,1,3,6,8};

    for (int col = 0; col < 5; ++col) {
        int curbit = curb[col];
        int diff = 0;

        /* up neighbours */
        if (up >= 0) {
            if (above[col] != curbit)     diff++;
            if (col > 0 && above[col-1] != curbit) diff++;
            if (col < 4 && above[col+1] != curbit) diff++;
        }
        /* same‑row neighbours */
        if (col > 0 && curb[col-1] != curbit) diff++;
        if (col < 4 && curb[col+1] != curbit) diff++;

        /* down neighbours */
        if (down >= 0) {
            if (below[col] != curbit)    diff++;
            if (col > 0 && below[col-1] != curbit) diff++;
            if (col < 4 && below[col+1] != curbit) diff++;
        }

        int ok = 0;
        for (int k = 0; k < 5; ++k)
            if (diff == allowed[k]) { ok = 1; break; }
        if (!ok) return 0;   /* cell violates the rule */
    }
    return 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle n=0 (out of offset) */
    if (n == 0) return 0;

    /* Special case n==1: evaluate each row directly */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < 32; ++row) {
            if (row & 1) continue;               /* left‑most element must be 0 */
            if (is_valid_row(-1, row, -1)) total += 1;
        }
        return total;
    }

    /* Pre‑computed transitions: for each (up,cur) pair, list of next rows
       that keep 'cur' valid. Computed only once. */
    static int next_rows[1024][32];
    static size_t next_len[1024];
    static int precomp_done = 0;
    if (!precomp_done) {
        for (int up = 0; up < 32; ++up)
            for (int cur = 0; cur < 32; ++cur) {
                size_t idx = (up << 5) | cur;
                size_t len = 0;
                for (int nxt = 0; nxt < 32; ++nxt)
                    if (is_valid_row(up, cur, nxt))
                        next_rows[idx][len++] = nxt;
                next_len[idx] = len;
            }
        precomp_done = 1;
    }

    /* DP arrays: count[ up<<5 | cur ] holds number of ways to reach this pair
       after processing the current row.  We only need two arrays. */
    seq_t count[1024] = {0}, ncount[1024] = {0};

    /* Initialise with the first two rows */
    for (int row0 = 0; row0 < 32; ++row0) {
        if (row0 & 1) continue;          /* upper‑left element is 0 */
        for (int row1 = 0; row1 < 32; ++row1)
            if (is_valid_row(-1, row0, row1))
                count[(row0 << 5) | row1] += 1;
    }

    /* Process rows 2 .. n‑1 (indexes since we already have row0, row1). */
    for (uint64_t r = 2; r < n; ++r) {
        for (size_t i = 0; i < 1024; ++i) ncount[i] = 0;
        for (size_t s = 0; s < 1024; ++s) {
            seq_t curcnt = count[s];
            if (!curcnt) continue;
            size_t len = next_len[s];
            for (size_t k = 0; k < len; ++k) {
                int nxt = next_rows[s][k];
                size_t ns = ((s & 31) << 5) | nxt;   /* new state (cur,nxt) */
                ncount[ns] += curcnt;
            }
        }
        for (size_t i = 0; i < 1024; ++i) count[i] = ncount[i];
    }

    /* Final validation of the last row (row n‑1) */
    seq_t result = 0;
    for (size_t s = 0; s < 1024; ++s) {
        seq_t ways = count[s];
        if (!ways) continue;
        int prev2 = s >> 5;
        int prev1 = s & 31;
        if (is_valid_row(prev2, prev1, -1))
            result += ways;
    }

    return result;
}

/*
 * A297549  Number of 6Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A297549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A297549 using dynamic programming over
   column pairs (prev, curr).  The array is treated column‑wise,
   and a 1 in the middle column must have 2 or 3 neighboring 1s
   from the left and right columns.  For n=1 the only valid
   configuration is all zeros.  For n≥2 the DP iterates over
   the columns.  Complexity: O(n·M·D) where M=4096 (64×64)
   and D is the average branching factor (~10).  Memory use
   is O(M). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Assumes seq_t and idx_t are typedefed by the build system. */
seq_t a(idx_t n)
{
    /* Small constants. */
    const int ROWS = 6;
    const int MASKS = 1 << ROWS;          /* 64 */
    const int STATES = MASKS * MASKS;     /* 4096 */

    /* Static data are initialised only once. */
    static bool initialized = false;

    /* Validity table: good[prev][curr][next] == true if
       the middle mask “curr” is valid when its neighbours
       are prev (left) and next (right). */
    static unsigned char good[64][64][64];
    /* Adjacency lists: for each state (prev,curr) store all
       next masks that keep the middle column valid. */
    static unsigned char next_masks[4096][64];
    static unsigned char next_counts[4096];
    /* DP containers.  They are static to avoid repeated allocation. */
    static seq_t dp[4096];
    static seq_t dp_next[4096];

    if (!initialized) {
        /* Precompute good table and adjacency lists. */
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                unsigned char *list = next_masks[prev * MASKS + curr];
                unsigned char cnt = 0;
                for (int next = 0; next < 64; ++next) {
                    bool ok = true;
                    /* Check each 1 in the middle mask. */
                    for (int r = 0; r < ROWS; ++r) {
                        if (!(curr & (1 << r))) continue;
                        int neigh = 0;
                        /* Left neighbours */
                        if (prev & (1 << r)) neigh++;
                        if (r > 0 && (prev & (1 << (r - 1)))) neigh++;
                        if (r < ROWS - 1 && (prev & (1 << (r + 1)))) neigh++;
                        /* Right neighbours */
                        if (next & (1 << r)) neigh++;
                        if (r > 0 && (next & (1 << (r - 1)))) neigh++;
                        if (r < ROWS - 1 && (next & (1 << (r + 1)))) neigh++;
                        if (neigh != 2 && neigh != 3) { ok = false; break; }
                    }
                    good[prev][curr][next] = ok;
                    if (ok) list[cnt++] = (unsigned char)next;
                }
                next_counts[prev * MASKS + curr] = cnt;
            }
        }
        initialized = true;
    }

    /* Special case n == 1: only all‑zeros array. */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Initialise DP for the first column.  The virtual left
       neighbour for column 1 is mask 0. */
    memset(dp, 0, sizeof(dp));
    for (int mask1 = 0; mask1 < 64; ++mask1) {
        dp[mask1] = (seq_t)1;   /* state index: prev=0, curr=mask1 */
    }

    /* Process columns 2..n.  After processing column j,
       dp[state] holds counts for configurations of columns
       1..j where the last two columns are represented
       by the state (prev,curr).  All columns up to j-1
       already satisfy constraints. */
    for (idx_t col = 2; col <= n; ++col) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int state = 0; state < STATES; ++state) {
            seq_t val = dp[state];
            if (!val) continue;
            unsigned char *list = next_masks[state];
            unsigned char cnt   = next_counts[state];
            /* curr is low 6 bits of state. */
            int curr = state & 63;
            for (unsigned char i = 0; i < cnt; ++i) {
                int next = list[i];
                int new_state = (curr << 6) | next;
                dp_next[new_state] += val;
            }
        }
        /* Move to the next step. */
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Final column: its right neighbour is mask 0. */
    seq_t ans = (seq_t)0;
    for (int state = 0; state < STATES; ++state) {
        seq_t val = dp[state];
        if (!val) continue;
        int prev = state >> 6;
        int curr = state & 63;
        if (good[prev][curr][0]) {
            ans += val;
        }
    }
    return ans;
}

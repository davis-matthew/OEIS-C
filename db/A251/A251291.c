/*
 * A251291  Number of (n+1)X(7+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge or main diagonally.
 * https://oeis.org/A251291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251291: Number of (n+1)×8 0/1 arrays whose every 2×2 subblock
 *  contains either a single 1 or exactly two 1s that are adjacent
 *  (horizontally or vertically) or lie on the main diagonal (top‑left
 *  and bottom‑right).  The implementation uses a transfer‑matrix over
 *  8‑bit row states and counts the number of admissible arrays of
 *  height n+1. */
#include <stdint.h>
#include <stdbool.h>

/* for each pair of rows (s → t) we store whether the transition is
   allowed; nexts[s] holds the list of admissible t values and
   deg[s] is its length.  The table is built once. */
static bool nexts_initialized = false;
static int nexts[256][256];
static int deg[256];

/* test whether a single 2×2 block with corners (tl,tr,bl,br) is allowed */
static inline bool block_ok(int tl, int tr, int bl, int br)
{
    int c = tl + tr + bl + br;
    if (c == 1) return true;                 /* exactly one 1 */
    if (c != 2) return false;                /* 0,3,4 ones are forbidden */

    /* two 1s are allowed if adjacent or on the main diagonal */
    if (tl && tr) return true;   /* top edge   */
    if (bl && br) return true;   /* bottom edge*/
    if (tl && bl) return true;   /* left edge  */
    if (tr && br) return true;   /* right edge */
    if (tl && br) return true;   /* main diagonal */
    return false;                /* all other pairs illegal */
}

/* build adjacency lists once */
static void init_nexts(void)
{
    for (int s = 0; s < 256; ++s) {
        deg[s] = 0;
        for (int t = 0; t < 256; ++t) {
            bool ok = true;
            for (int j = 0; j < 7; ++j) {
                int tl = (s >> j)        & 1;
                int tr = (s >> (j+1))    & 1;
                int bl = (t >> j)        & 1;
                int br = (t >> (j+1))    & 1;
                if (!block_ok(tl, tr, bl, br)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                nexts[s][deg[s]++] = t;
            }
        }
    }
    nexts_initialized = true;
}

/* the public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset – not needed */

    if (!nexts_initialized) init_nexts();

    static seq_t cur[256];
    static seq_t nxt[256];
    /* initialise: first row can be arbitrary */
    for (int i = 0; i < 256; ++i) cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        /* reset next vector */
        for (int i = 0; i < 256; ++i) nxt[i] = 0;

        /* propagate transitions */
        for (int s = 0; s < 256; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int k = 0; k < deg[s]; ++k) {
                int t = nexts[s][k];
                nxt[t] += val;
            }
        }
        /* move to next state */
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    /* sum all possible last rows */
    seq_t total = 0;
    for (int i = 0; i < 256; ++i) total += cur[i];
    return total;
}

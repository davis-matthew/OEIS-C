/*
 * A260471  Number of (n+2)X(4+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000000 00000001 or 00010101.
 * https://oeis.org/A260471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   build_allowed_masks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260471: a(n) is the number of (n+2)×6 binary arrays
   in which every 3×3 subblock has a clockwise perimeter pattern
   equal to 00000000, 00000001, or 00010101 (or any rotation of these
   masks).  The values are computed by dynamic programming over
   consecutive rows; the recurrence itself is not explicit. */

#include <stdint.h>
#include "../utils/build_allowed_masks.h"

static int transitions_built = 0;

/* ------------------------------------------------------------------
   Build transition tables between pairs of consecutive rows.
   State is a pair (r0, r1) encoded as r0<<6 | r1 (each row is 6 bits).
   For each state we compute all possible following rows r2 that keep
   every 3×3 block (in columns 0‑3) with the perimeter in the allowed set.
   ------------------------------------------------------------------ */
static uint32_t trans_start[4097];      /* start index in trans_vec for state */
static uint8_t  trans_len [4096];       /* number of transitions from state   */
static uint8_t  trans_vec [262144];     /* flattened list of next rows (r2)    */

static void build_transitions(void)
{
    if (transitions_built) return;
    transitions_built = 1;

    uint8_t allowed[256];
    build_allowed_masks(allowed);

    uint32_t pos = 0;
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int r1 = 0; r1 < 64; ++r1) {
            int idx = (r0 << 6) | r1;
            uint32_t start = pos;

            for (int r2 = 0; r2 < 64; ++r2) {
                /* check all 4 3×3 subblocks that involve rows r0,r1,r2 */
                int ok = 1;
                for (int c = 0; c < 4 && ok; ++c) {
                    int b0 = (r0 >>  c) & 1;             /* a */
                    int b1 = (r0 >> (c+1)) & 1;          /* b */
                    int b2 = (r0 >> (c+2)) & 1;          /* c */
                    int b3 = (r1 >> (c+2)) & 1;          /* f */
                    int b4 = (r2 >> (c+2)) & 1;          /* i */
                    int b5 = (r2 >> (c+1)) & 1;          /* h */
                    int b6 = (r2 >>  c) & 1;             /* g */
                    int b7 = (r1 >>  c) & 1;             /* d */

                    uint8_t pat = (uint8_t)((b0<<7)|(b1<<6)|(b2<<5)|(b3<<4)
                                         |(b4<<3)|(b5<<2)|(b6<<1)|b7);
                    if (!allowed[pat]) { ok = 0; break; }
                }

                if (ok) trans_vec[pos++] = (uint8_t)r2;
            }

            trans_len[idx]  = (uint8_t)(pos - start);
            trans_start[idx+1] = pos;
        }
    }
}

/* ------------------------------------------------------------------
   Dynamic programming over rows.
   dp[state]  – number of ways to build all rows processed so far
   with the last two rows encoded by 'state'.
   ------------------------------------------------------------------ */
static seq_t dp[4096];
static seq_t nextdp[4096];

seq_t a(idx_t n)
{
    /* n is 1‑based: n = 1 corresponds to 3 rows (the minimal valid array) */
    build_transitions();

    /* number of rows to append after the initial two arbitrary rows */
    idx_t steps = n;               /* total rows = n+2 */

    seq_t *cur = dp;
    seq_t *nxt = nextdp;

    /* initial two rows can be any 6‑bit pattern */
    for (int i = 0; i < 4096; ++i) cur[i] = 1;

    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < 4096; ++i) nxt[i] = 0;

        for (int old = 0; old < 4096; ++old) {
            seq_t val = cur[old];
            if (!val) continue;

            uint32_t start = trans_start[old];
            uint8_t  len   = trans_len[old];
            int prev = old & 63;                /* second row of the old state */

            for (int t = 0; t < len; ++t) {
                uint8_t r2 = trans_vec[start + t];
                int new_state = (prev << 6) | r2;
                nxt[new_state] += val;
            }
        }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all final states */
    seq_t result = 0;
    for (int i = 0; i < 4096; ++i) result += cur[i];
    return result;
}

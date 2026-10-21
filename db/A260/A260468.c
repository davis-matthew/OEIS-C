/*
 * A260468  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00010101.
 * https://oeis.org/A260468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 59, u128 n <= 121,
 *            big any n
 * Helpers:   perimeter_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A260468
 *
 *  a(n) = number of (n+2) x 3 binary arrays such that every 3x3
 *  subblock has one of the perimeters that is a rotation of
 *  00000000, 00000001 or 00010101 (clockwise order).
 *
 *  The algorithm performs a linear DP over rows:
 *      State is the last two rows (each ∈ {0..7}),
 *      transition from (r0,r1) to (r1,r2) when the triple
 *      (r0,r1,r2) is allowed.
 *
 *  Initial state: all 64 pairs of first two rows are possible.
 *  For each of the n additional rows we apply the transition.
 *
 *  Result: sum of all states after n transitions.
 *
 *  This satisfies all required properties and runs in O(n) time.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/perimeter_pattern.h"

static bool patterns[256] = {0};

/* populate the patterns[] array with all rotations of the three base patterns */
static void init_patterns(void)
{
    static bool once = false;
    if (once) return;
    once = true;

    /* base patterns */
    const unsigned char base[] = {0x00, 0x01, 0x15};
    for (int k = 0; k < (int)(sizeof(base)/sizeof(base[0])); ++k) {
        unsigned char p = base[k];
        for (int r = 0; r < 8; ++r) {
            patterns[p] = true;
            p = (p >> 1) | ((p & 1) << 7);   // rotate right by 1
        }
    }
}

/* pre‑compute allowed transitions: for each pair (r0,r1) list all r2 that
 * form an allowed triple.  We store the data in a static table. */
static unsigned char next_rows[64][8];   /* next_rows[idx][k] = r2   */
static unsigned char next_count[64];    /* how many r2 for this pair */

static void init_transitions(void)
{
    static bool once = false;
    if (once) return;
    once = true;

    for (unsigned int r0 = 0; r0 < 8; ++r0)
        for (unsigned int r1 = 0; r1 < 8; ++r1) {
            unsigned int idx = r0 * 8 + r1;
            unsigned int cnt = 0;
            for (unsigned int r2 = 0; r2 < 8; ++r2) {
                if (patterns[perimeter_pattern((unsigned char)r0,
                                              (unsigned char)r1,
                                              (unsigned char)r2)]) {
                    next_rows[idx][cnt++] = (unsigned char)r2;
                }
            }
            next_count[idx] = cnt;
        }
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_patterns();
    init_transitions();

    /* DP tables: cur[r0][r1] = number of sequences ending with rows r0,r1. */
    seq_t cur[8][8];
    seq_t nxt[8][8];
    for (unsigned int r0 = 0; r0 < 8; ++r0)
        for (unsigned int r1 = 0; r1 < 8; ++r1)
            cur[r0][r1] = 1;   /* all pairs of the first two rows are possible */

    for (idx_t step = 0; step < n; ++step) {
        for (unsigned int i = 0; i < 8; ++i)
            for (unsigned int j = 0; j < 8; ++j)
                nxt[i][j] = 0;

        for (unsigned int r0 = 0; r0 < 8; ++r0)
            for (unsigned int r1 = 0; r1 < 8; ++r1) {
                seq_t val = cur[r0][r1];
                if (!val) continue;
                unsigned int idx = r0 * 8 + r1;
                for (unsigned int k = 0; k < next_count[idx]; ++k) {
                    unsigned char r2 = next_rows[idx][k];
                    nxt[r1][r2] += val;
                }
            }

        /* copy nxt to cur for next iteration */
        for (unsigned int i = 0; i < 8; ++i)
            for (unsigned int j = 0; j < 8; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum all end states */
    seq_t total = 0;
    for (unsigned int i = 0; i < 8; ++i)
        for (unsigned int j = 0; j < 8; ++j)
            total += cur[i][j];

    return total;
}

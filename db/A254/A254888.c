/*
 * A254888  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          sum plus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A254888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 14, u64 n <= 43, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254888: number of (n+2)x4 binary arrays whose
   derived n×2 matrix is monontone.  The derived value for a
   3×3 subblock is diagonal_sum + antidiagonal_median.
   The DP iterates over row triples; state = (last two rows,
   last derived row).  Each row is encoded as 4 bits (value 0..15).  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: precompute derived values for all row triples. */
static unsigned char triple_f1[1 << 12]; /* diag_sum + median, offset 0 */
static unsigned char triple_f2[1 << 12]; /* diag_sum + median, offset 1 */

static unsigned char state_r1[16 * 16 * 5 * 5];
static unsigned char state_r2[16 * 16 * 5 * 5];
static unsigned char state_f1[16 * 16 * 5 * 5];
static unsigned char state_f2[16 * 16 * 5 * 5];

/* Decode formula for state index:
   idx = ((r1*16 + r2)*5 + f1)*5 + f2. */
static void init_states(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Precompute derived values for all 3-row triples (16^3 = 4096). */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2)
            for (int r3 = 0; r3 < 16; ++r3) {
                int idx = (r1 << 8) | (r2 << 4) | r3;

                /* Offset 0 (columns 0–2). */
                int diag0  = ((r1 >> 0) & 1) + ((r2 >> 1) & 1) + ((r3 >> 2) & 1);
                int ant0   = ((r1 >> 2) & 1) + ((r2 >> 1) & 1) + ((r3 >> 0) & 1);
                int maj0   = (ant0 >= 2) ? 1 : 0;
                triple_f1[idx] = (unsigned char)(diag0 + maj0);

                /* Offset 1 (columns 1–3). */
                int diag1  = ((r1 >> 1) & 1) + ((r2 >> 2) & 1) + ((r3 >> 3) & 1);
                int ant1   = ((r1 >> 3) & 1) + ((r2 >> 2) & 1) + ((r3 >> 1) & 1);
                int maj1   = (ant1 >= 2) ? 1 : 0;
                triple_f2[idx] = (unsigned char)(diag1 + maj1);
            }

    /* Precompute decode tables for states (16×16×5×5 = 6400). */
    for (size_t idx = 0; idx < 16ULL * 16 * 5 * 5; ++idx) {
        size_t t = idx;
        state_f2[idx] = (unsigned char)(t % 5); t /= 5;
        state_f1[idx] = (unsigned char)(t % 5); t /= 5;
        state_r2[idx] = (unsigned char)(t % 16); t /= 16;
        state_r1[idx] = (unsigned char)(t);
    }
}

/* DP arrays: one for current step, one for next. */
static seq_t dp_state[16 * 16 * 5 * 5];
static seq_t dp_state_next[16 * 16 * 5 * 5];
static bool active_state[16 * 16 * 5 * 5];
static bool active_state_next[16 * 16 * 5 * 5];

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1 but just in case */
    init_states();

    /* dp_row[r1*16 + r2] = 1 for all 256 pairs of initial rows. */
    static seq_t dp_row[16 * 16];
    for (int i = 0; i < 256; ++i) dp_row[i] = 1;

    /* Process the first block separately to create state space. */
    size_t steps = (size_t)n;
    for (size_t step = 1; step <= steps; ++step) {
        /* clear next DP and active flags */
        for (size_t i = 0; i < 6400; ++i) {
            dp_state_next[i] = 0;
            active_state_next[i] = false;
        }

        if (step == 1) {
            /* Use dp_row to form the first block. */
            for (int r1 = 0; r1 < 16; ++r1)
                for (int r2 = 0; r2 < 16; ++r2) {
                    seq_t cnt = dp_row[r1 * 16 + r2];
                    if (cnt == 0) continue;
                    for (int r3 = 0; r3 < 16; ++r3) {
                        unsigned char f1 = triple_f1[(r1 << 8) | (r2 << 4) | r3];
                        unsigned char f2 = triple_f2[(r1 << 8) | (r2 << 4) | r3];
                        if (f1 <= f2) {
                            size_t idx = ((r2 * 16 + r3) * 5 + f1) * 5 + f2;
                            dp_state_next[idx] += cnt;
                            active_state_next[idx] = true;
                        }
                    }
                }
        } else {
            /* Use existing states. */
            for (size_t idx = 0; idx < 6400; ++idx) {
                if (!active_state[idx]) continue;
                seq_t cnt = dp_state[idx];
                unsigned char r1 = state_r1[idx];
                unsigned char r2 = state_r2[idx];
                unsigned char f1 = state_f1[idx];
                unsigned char f2 = state_f2[idx];
                for (int r3 = 0; r3 < 16; ++r3) {
                    unsigned char new_f1 = triple_f1[(r1 << 8) | (r2 << 4) | r3];
                    unsigned char new_f2 = triple_f2[(r1 << 8) | (r2 << 4) | r3];
                    if (f1 <= new_f1 && f2 <= new_f2 && new_f1 <= new_f2) {
                        size_t nidx = ((r2 * 16 + r3) * 5 + new_f1) * 5 + new_f2;
                        dp_state_next[nidx] += cnt;
                        active_state_next[nidx] = true;
                    }
                }
            }
        }

        /* swap current and next states */
        for (size_t i = 0; i < 6400; ++i) {
            dp_state[i] = dp_state_next[i];
            active_state[i] = active_state_next[i];
        }
    }

    /* Sum all counts after processing all blocks. */
    seq_t result = 0;
    for (size_t i = 0; i < 6400; ++i) {
        if (active_state[i]) result += dp_state[i];
    }
    return result;
}

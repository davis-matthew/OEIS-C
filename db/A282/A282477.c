/*
 * A282477  Number of n X 4 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over rows. State:
 *            previous row mask + partial neighbor counts (up, left, right,
 *            up-left, up-right) for each cell. Transition: iterate over next
 *            row mask, update exception count, compute new partial counts.
 *            Complexity: O(16 * number_of_states * n), with stat...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_MASK 16             /* 4-bit column masks */
#define MAX_BITS (1 << 12)      /* 12 bits (3 bits per 4 columns) */
#define MAX_PATTERN_COUNT 4096  /* upper bound on pattern count */
#define MAX_STATES (3 * MAX_PATTERN_COUNT)

/* mapping from (mask,bits) to pattern id; -1 means invalid */
static short global_id[MAX_MASK][MAX_BITS];

/* inverse mapping for a pattern id */
static unsigned char  pattern_mask[MAX_PATTERN_COUNT];
static unsigned short pattern_bits[MAX_PATTERN_COUNT];
static int pattern_count;

/* build tables, called once */
static void build_tables(void)
{
    /* initialise mapping to -1 */
    for (int m = 0; m < MAX_MASK; ++m)
        for (int b = 0; b < MAX_BITS; ++b)
            global_id[m][b] = -1;

    pattern_count = 0;
    for (int mask = 0; mask < MAX_MASK; ++mask) {
        for (int bits = 0; bits < MAX_BITS; ++bits) {
            bool ok = true;
            for (int c = 0; c < 4; ++c) {
                int val = (bits >> (3 * c)) & 7;   /* 0..7 */
                if (!(mask & (1 << c))) {
                    if (val) { ok = false; break; }
                } else {
                    if (val > 5) { ok = false; break; }  /* partial count cannot exceed 5 */
                }
            }
            if (!ok) continue;
            global_id[mask][bits] = pattern_count;
            pattern_mask[pattern_count] = (unsigned char)mask;
            pattern_bits[pattern_count] = (unsigned short)bits;
            ++pattern_count;
        }
    }
}

/* DP arrays and active state lists */
static seq_t dp0[MAX_STATES], dp1[MAX_STATES];
static seq_t *dp = dp0, *next_dp = dp1;
static int active_arr[2][MAX_STATES];
static int *active = active_arr[0], *next_active = active_arr[1];
static int active_len = 0, next_active_len = 0;
static unsigned char seen[MAX_STATES];

/* Return the number of binary n x 4 arrays satisfying the condition. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        build_tables();
        initialized = true;
    }

    /* initialise DP */
    memset(dp, 0, sizeof(dp0));
    /* start with previous row empty */
    int start_id = pattern_count * 0 + global_id[0][0];
    dp[start_id] = 1;
    active[0] = start_id;
    active_len = 1;

    for (idx_t row = 0; row < n; ++row) {
        /* reset next DP and seen */
        memset(next_dp, 0, sizeof(dp0));
        memset(seen, 0, sizeof(seen));
        next_active_len = 0;

        /* process all active states */
        for (int ai = 0; ai < active_len; ++ai) {
            int sid = active[ai];
            int exc = sid / pattern_count;           /* 0..2 */
            int pid = sid % pattern_count;           /* pattern id of previous row */
            unsigned short prev_bits = pattern_bits[pid];
            unsigned char   prev_mask = pattern_mask[pid];

            for (int cur_mask = 0; cur_mask < MAX_MASK; ++cur_mask) {
                /* compute exceptions added by completing previous row */
                int exc_added = 0;
                for (int c = 0; c < 4; ++c) {
                    if (!(prev_mask & (1 << c))) continue;

                    int pc_partial = (prev_bits >> (3 * c)) & 7;
                    int down_l = (c > 0)      ? ((cur_mask >> (c - 1)) & 1) : 0;
                    int down   =              ((cur_mask >> c)      & 1);
                    int down_r = (c < 3)      ? ((cur_mask >> (c + 1)) & 1) : 0;
                    if (pc_partial + down_l + down + down_r > 3)
                        ++exc_added;
                }

                int new_exc = exc + exc_added;
                if (new_exc > 2) continue;

                /* compute new partial counts for current row */
                unsigned short new_bits = 0;
                for (int c = 0; c < 4; ++c) {
                    if (!(cur_mask & (1 << c))) continue;

                    int up   = (prev_mask >> c)      & 1;
                    int up_l = (c > 0)           ? ((prev_mask >> (c - 1)) & 1) : 0;
                    int up_r = (c < 3)           ? ((prev_mask >> (c + 1)) & 1) : 0;
                    int left = (c > 0)           ? ((cur_mask >> (c - 1)) & 1) : 0;
                    int right= (c < 3)           ? ((cur_mask >> (c + 1)) & 1) : 0;
                    int pc = up + up_l + up_r + left + right;
                    new_bits |= (pc << (3 * c));
                }

                int new_pid = global_id[cur_mask][new_bits];
                int new_sid = new_exc * pattern_count + new_pid;

                next_dp[new_sid] += dp[sid];
                if (!seen[new_sid]) {
                    seen[new_sid] = 1;
                    next_active[next_active_len++] = new_sid;
                }
            }
        }

        /* swap DP arrays and active lists for next iteration */
        seq_t *tmp_dp = dp;
        dp = next_dp;
        next_dp = tmp_dp;

        int *tmp_act = active;
        active = next_active;
        next_active = tmp_act;
        active_len = next_active_len;
    }

    /* final flush: evaluate the last row's neighbor counts */
    seq_t result = 0;
    for (int ai = 0; ai < active_len; ++ai) {
        int sid = active[ai];
        int exc = sid / pattern_count;
        int pid = sid % pattern_count;
        unsigned short part_bits = pattern_bits[pid];
        unsigned char   part_mask = pattern_mask[pid];

        int exc_final = 0;
        for (int c = 0; c < 4; ++c) {
            if (!(part_mask & (1 << c))) continue;
            int part = (part_bits >> (3 * c)) & 7;
            if (part > 3) ++exc_final;
        }

        if (exc + exc_final == 2)
            result += dp[sid];
    }

    return result;
}

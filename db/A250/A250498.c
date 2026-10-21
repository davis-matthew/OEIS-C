/*
 * A250498  Number of (n+2)X(2+2) 0..1 arrays with nondecreasing medians of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Method: Dynamic programming over rows.
    The algorithm counts (n+2)×4 binary arrays whose rows and columns satisfy
    the “nondecreasing medians of every three consecutive values” property.
    For each row we enforce the row constraint, and for each column we keep
    the last two bits and the previous median.  The DP has 4^4×2^4 = 4096
    states and is O((n+2)·4096·|valid_rows|).  This runs in well under two
    seconds for n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool row_ok[16];
static int  valid_rows[16];
static int  valid_cnt = 0;

/* transition[s][p] = next state after adding row pattern p
   state s is encoded as 3 bits per column (lower 2 bits = last two bits,
   LSB = previous median).  The value is -1 if the transition is forbidden. */
static int16_t transition_tbl[4096][16];
static bool tbl_ready = false;

/* Compute row_ok array and valid_rows list */
static void init_row_ok(void)
{
    for (int p = 0; p < 16; ++p) {
        int b[4];
        for (int i = 0; i < 4; ++i) b[i] = (p >> i) & 1;
        int sum1 = b[0] + b[1] + b[2];
        int sum2 = b[1] + b[2] + b[3];
        row_ok[p] = !(sum1 >= 2 && sum2 < 2);          /* median of 1-3 <= median of 2-4 */
        if (row_ok[p]) {
            valid_rows[valid_cnt++] = p;
        }
    }
}

/* Build the transition table once */
static void init_transition_table(void)
{
    for (int s = 0; s < 4096; ++s) {
        for (int p = 0; p < 16; ++p) {
            if (!row_ok[p]) {                    /* this row pattern fails row rule */
                transition_tbl[s][p] = -1;
                continue;
            }
            bool ok = true;
            int newstate = 0;
            for (int c = 0; c < 4; ++c) {
                int seg = (s >> (3 * c)) & 7;   /* 3 bits for column c */
                int last_bits = seg >> 1;       /* upper 2 bits */
                int prev      = seg & 1;        /* lower bit */
                int bit_old   = (last_bits >> 1) & 1;   /* older row */
                int bit_newer = last_bits & 1;          /* newest row (i-1) */
                int newbit    = (p >> c) & 1;           /* new row (i) */
                int sum = bit_old + bit_newer + newbit;
                int med = (sum >= 2) ? 1 : 0;
                if (med < prev) { ok = false; break; } /* nondecreasing median */
                int new_last_bits = ((bit_newer << 1) | newbit);
                int new_prev = med;
                int new_seg = (new_last_bits << 1) | new_prev;
                newstate |= (new_seg << (3 * c));
            }
            transition_tbl[s][p] = ok ? (int16_t)newstate : -1;
        }
    }
    tbl_ready = true;
}

/* Main function */
seq_t a(idx_t n)
{
    if (!tbl_ready) { init_row_ok(); init_transition_table(); }

    /* Number of rows (m) = n + 2 */
    idx_t m = n + 2;
    if (m < 3) {                   /* for completeness; not needed per spec */
        return 0;
    }

    /* Allocate DP arrays */
    seq_t *dp     = (seq_t *)malloc(4096 * sizeof(seq_t));
    seq_t *nextdp = (seq_t *)malloc(4096 * sizeof(seq_t));
    if (!dp || !nextdp) return 0;   /* allocation failure; unlikely */

    memset(dp, 0, 4096 * sizeof(seq_t));
    memset(nextdp, 0, 4096 * sizeof(seq_t));

    /* Initialise with all valid pairs of first two rows */
    for (int i = 0; i < valid_cnt; ++i) {
        int r1 = valid_rows[i];
        for (int j = 0; j < valid_cnt; ++j) {
            int r2 = valid_rows[j];
            int state = 0;
            for (int c = 0; c < 4; ++c) {
                int a = (r1 >> c) & 1;
                int b = (r2 >> c) & 1;
                int last_bits = (a << 1) | b;   /* older (r1), newer (r2) */
                int seg = (last_bits << 1);     /* previous median = 0 */
                state |= (seg << (3 * c));
            }
            dp[state] += 1;                     /* one way to reach this state */
        }
    }

    /* Process remaining rows (m-2 of them) */
    idx_t remaining = m - 2;
    for (idx_t step = 0; step < remaining; ++step) {
        memset(nextdp, 0, 4096 * sizeof(seq_t));
        for (int s = 0; s < 4096; ++s) {
            seq_t cnt = dp[s];
            if (cnt == 0) continue;
            for (int vi = 0; vi < valid_cnt; ++vi) {
                int p = valid_rows[vi];
                int16_t ns = transition_tbl[s][p];
                if (ns >= 0) {
                    nextdp[ns] += cnt;
                }
            }
        }
        /* swap dp and nextdp */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
    }

    /* Sum all counts in dp */
    seq_t result = 0;
    for (int s = 0; s < 4096; ++s) {
        result += dp[s];
    }

    free(dp);
    free(nextdp);
    return result;
}

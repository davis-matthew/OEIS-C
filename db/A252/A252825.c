/*
 * A252825  Number of nX5 nonnegative integer arrays with upper left 0 and
 *          every value within 3 of its city block distance from the upper
 *          left and every value increasing by 0 or 1 with every step right or
 *          down.
 * https://oeis.org/A252825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×5 arrays with the constraints described in A252825
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 79, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses incremental dynamic programming over the rows.
   For each row we enumerate all possible 5‑tuples that satisfy
   the “within 3 of the Manhattan distance” condition and the
   internal “+0/+1” condition.  
   We then count the ways to extend a previous row to a current row
   by checking the vertical “+0/+1” rule.  The sequence values a(1…n)
   are stored in a static array so that repeated calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* a row state (values of the 5 columns) */
typedef struct { int v[5]; } RowState;

/* maximum number of distinct row states – 256 is ample for all n */
#define MAX_STATES 256

/* maximum n expected during tests (≈10000) – a safe upper bound */
#define MAX_N 20000

/* ------------- global static storage ----------------- */
static seq_t A[MAX_N + 1];          /* sequence values  (a(1) stored at A[1]) */
static idx_t last_computed_n = 0;   /* largest n already computed */

/* two buffers that we toggle each iteration so that we never
   allocate or free memory during the computation */
static RowState stateA[MAX_STATES];
static RowState stateB[MAX_STATES];
static seq_t   countA[MAX_STATES];
static seq_t   countB[MAX_STATES];

static RowState  *prev_states = stateA;              /* states of the last row processed */
static seq_t     *prev_counts = countA;
static RowState  *curr_states = stateB;              /* workspace for the current row */
static seq_t     *curr_counts = countB;
static size_t    prev_cnt = 0;                       /* number of states in prev_states */

/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 – values for n>=1 are defined */

    /* if we already have the requested value, just return it */
    if (n == last_computed_n) return A[n];

    /* extend the computation up to n, row by row */
    for (idx_t row = last_computed_n + 1; row <= n; ++row)
    {
        /* ---- bounds for the current row --------------------------------- */
        int low[5], high[5];
        idx_t base_row = row - 1;                 /* zero‑based row index */
        for (int j = 0; j < 5; ++j)
        {
            idx_t dist = base_row + j;           /* Manhattan distance of (row, j) */
            low[j]  = (int)(dist > 3 ? dist - 3 : 0);
            high[j] = (int)(dist + 3);
        }

        size_t curr_state_cnt = 0;                /* number of row states we keep */

        if (row == 1)                            /* first row – upper left must be 0 */
        {
            int base = 0;                        /* row[0] must be 0 */
            for (int mask = 0; mask < 16; ++mask)
            {
                RowState rs;
                int val = base;
                rs.v[0] = val;

                bool ok = true;
                for (int j = 1; j < 5; ++j)
                {
                    val += (mask >> (j-1)) & 1;
                    rs.v[j] = val;
                }

                for (int j = 0; ok && j < 5; ++j)
                    if (rs.v[j] < low[j] || rs.v[j] > high[j]) ok = false;

                if (ok)
                {
                    curr_states[curr_state_cnt] = rs;
                    curr_counts[curr_state_cnt] = 1;          /* one way to form this row */
                    ++curr_state_cnt;
                }
            }
        }
        else                                      /* subsequent rows */
        {
            int base_low  = (row - 4 > 0 ? row - 4 : 0);  /* lower bound for column 0   */
            int base_high = row + 2;                     /* upper bound for column 0   */

            for (int base = base_low; base <= base_high; ++base)
            {
                for (int mask = 0; mask < 16; ++mask)
                {
                    RowState rs;
                    int val = base;
                    rs.v[0] = val;

                    bool ok = true;
                    for (int j = 1; j < 5; ++j)
                    {
                        val += (mask >> (j-1)) & 1;
                        rs.v[j] = val;
                    }

                    for (int j = 0; ok && j < 5; ++j)
                        if (rs.v[j] < low[j] || rs.v[j] > high[j]) ok = false;

                    if (ok)
                    {
                        curr_states[curr_state_cnt] = rs;
                        curr_counts[curr_state_cnt] = 0;          /* will be filled below */
                        ++curr_state_cnt;
                    }
                }
            }

            /* transition from previous row to current row */
            for (size_t ci = 0; ci < curr_state_cnt; ++ci)
            {
                seq_t sum = 0;
                for (size_t pi = 0; pi < prev_cnt; ++pi)
                {
                    bool ok = true;
                    for (int j = 0; ok && j < 5; ++j)
                    {
                        int diff = curr_states[ci].v[j] - prev_states[pi].v[j];
                        if (diff < 0 || diff > 1) ok = false;
                    }
                    if (ok) sum += prev_counts[pi];
                }
                curr_counts[ci] = sum;
            }
        }

        /* ---- accumulate the number of arrays of size 'row' ---------------- */
        seq_t row_total = 0;
        for (size_t ci = 0; ci < curr_state_cnt; ++ci)
            row_total += curr_counts[ci];

        A[row] = row_total;

        /* ---- prepare for the next iteration -------------------------------- */
        RowState  *tmp_state  = prev_states;
        seq_t     *tmp_count  = prev_counts;

        prev_states = curr_states;
        prev_counts = curr_counts;
        curr_states = tmp_state;
        curr_counts = tmp_count;

        prev_cnt = curr_state_cnt;
    }

    last_computed_n = n;
    return A[n];
}

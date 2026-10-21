/*
 * A252107  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum equal to 0 2 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 57, u64 n <= 131, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A252107: Number of (n+2)×3 arrays with entries 0..3 such that
   every 3×3 subblock has each row and column sum in {0,2,3,6,7}
   and each diagonal and antidiagonal sum NOT in that set.
   Implementation: dynamic programming on rows; transition graph
   built once and reused for all n.  Uses a state = (row_i,row_{i+1})
   encoded as 12 bits (6 bits per row).  No hard‑coded table of terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Constants */
#define NUM_ROWS 64          /* 4^3 possible rows (0..3)^3 */
#define NUM_STATES (NUM_ROWS * NUM_ROWS)

/* Precomputed row digits and sums */
static unsigned char row_digits[NUM_ROWS][3];
static unsigned char row_sum[NUM_ROWS];

/* Transition lists: adjacency[state] holds next state indices */
static int *adjacency[NUM_STATES];
static int adjacency_len[NUM_STATES];
static int adjacency_cap[NUM_STATES];
static bool adjacency_ready = false;

/* Build the transition graph once */
static void build_adjacency(void)
{
    if (adjacency_ready) return;

    /* Compute digits for every row and its sum */
    for (int r = 0; r < NUM_ROWS; ++r) {
        row_digits[r][0] = r / 16;            /* most significant digit */
        row_digits[r][1] = (r / 4) % 4;
        row_digits[r][2] = r % 4;             /* least significant digit */
        row_sum[r] = row_digits[r][0] + row_digits[r][1] + row_digits[r][2];
    }

    /* Allowed sums for rows and columns */
    bool allowed[13] = {false};
    allowed[0] = allowed[2] = allowed[3] = allowed[6] = allowed[7] = true;

    /* Enumerate all triples of rows that satisfy the 3×3 constraints */
    for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
        if (!allowed[row_sum[r1]]) continue;
        for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
            if (!allowed[row_sum[r2]]) continue;
            for (int r3 = 0; r3 < NUM_ROWS; ++r3) {
                if (!allowed[row_sum[r3]]) continue;

                /* Column sums */
                bool ok = true;
                for (int c = 0; c < 3 && ok; ++c) {
                    int cs = row_digits[r1][c] + row_digits[r2][c] + row_digits[r3][c];
                    if (!allowed[cs]) ok = false;
                }
                if (!ok) continue;

                /* Diagonal sums must NOT be allowed */
                int d1 = row_digits[r1][0] + row_digits[r2][1] + row_digits[r3][2];
                if (d1 <= 12 && allowed[d1]) continue;
                int d2 = row_digits[r1][2] + row_digits[r2][1] + row_digits[r3][0];
                if (d2 <= 12 && allowed[d2]) continue;

                /* Record transition from (r1,r2) to (r2,r3) */
                int from = (r1 << 6) | r2;
                int to   = (r2 << 6) | r3;
                if (adjacency_len[from] == adjacency_cap[from]) {
                    int new_cap = adjacency_cap[from] == 0 ? 8 : adjacency_cap[from] * 2;
                    int *tmp = (int *)realloc(adjacency[from], new_cap * sizeof(int));
                    if (!tmp) exit(1);
                    adjacency[from] = tmp;
                    adjacency_cap[from] = new_cap;
                }
                adjacency[from][adjacency_len[from]++] = to;
            }
        }
    }

    adjacency_ready = true;
}

/* DP arrays, allocated once */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static bool dp_ready = false;

/* Number of (n+2)×3 arrays satisfying the conditions */
seq_t a(idx_t n)
{
    /* n corresponds to adding n rows beyond the initial two rows. */
    if (n == 0) return (seq_t)0;   /* offset is 1; n>=1 for real sequence */

    build_adjacency();

    /* Allocate DP arrays on first call */
    if (!dp_ready) {
        dp_cur   = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
        dp_next  = (seq_t *)malloc(NUM_STATES * sizeof(seq_t));
        if (!dp_cur || !dp_next) exit(1);
        dp_ready = true;
    }

    /* Initialize with all possible first two rows */
    for (int i = 0; i < NUM_STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* Add rows one by one */
    for (int step = 0; step < (int)n; ++step) {
        /* Zero dp_next */
        for (int i = 0; i < NUM_STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* Transition */
        for (int state = 0; state < NUM_STATES; ++state) {
            seq_t val = dp_cur[state];
            if (val == (seq_t)0) continue;

            int len = adjacency_len[state];
            int *list = adjacency[state];
            for (int k = 0; k < len; ++k) {
                int succ = list[k];
                dp_next[succ] += val;
            }
        }

        /* Swap DP arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over all states to get the total number of arrays */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NUM_STATES; ++i)
        total += dp_cur[i];

    return total;
}

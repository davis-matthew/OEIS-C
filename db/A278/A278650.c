/*
 * A278650  Number of nX2 0..2 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly three mistakes.
 * https://oeis.org/A278650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 34, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (DP): a(n) = number of n×2 arrays over {0,1,2} with rows
   in nondecreasing lexicographic order, columns in nonincreasing
   lexicographic order, with exactly three mistakes. */
#include <stdbool.h>

#define NUM_ROWS 9
#define MAX_MISTAKES 3
#define NUM_STATUS 3  /* 0: equal so far, 1: column0 > column1, 2: column0 < column1 */

/* Precomputed rows: each row is a pair (a,b) with a,b∈{0,1,2}. */
static int row_first[NUM_ROWS];
static int row_second[NUM_ROWS];
static int row_start_status[NUM_ROWS];

/* Initialise the arrays once. */
static void init_row_arrays(void)
{
    static bool done = false;
    if (done) return;
    done = true;
    for (int r = 0; r < NUM_ROWS; ++r) {
        int a = r / 3;          /* first column value */
        int b = r % 3;          /* second column value */
        row_first[r] = a;
        row_second[r] = b;
        if (a > b)      row_start_status[r] = 1;
        else if (a < b) row_start_status[r] = 2;
        else            row_start_status[r] = 0;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init_row_arrays();

    /* DP: cur[v][s][p] = number of ways to reach the current row
       with v row‑mistakes, column status s, ending with row type p. */
    seq_t cur[MAX_MISTAKES + 1][NUM_STATUS][NUM_ROWS] = {0};
    seq_t next[MAX_MISTAKES + 1][NUM_STATUS][NUM_ROWS] = {0};

    /* Initial row */
    for (int p = 0; p < NUM_ROWS; ++p) {
        int s = row_start_status[p];
        cur[0][s][p] += (seq_t)1;
    }

    /* Process subsequent rows (n‑1 transitions) */
    for (idx_t step = 1; step < n; ++step) {
        for (int v = 0; v <= MAX_MISTAKES; ++v)
            for (int s = 0; s < NUM_STATUS; ++s)
                for (int p = 0; p < NUM_ROWS; ++p)
                    next[v][s][p] = (seq_t)0;

        for (int v = 0; v <= MAX_MISTAKES; ++v) {
            for (int s = 0; s < NUM_STATUS; ++s) {
                for (int p = 0; p < NUM_ROWS; ++p) {
                    seq_t cur_val = cur[v][s][p];
                    if (!cur_val) continue;

                    int prev_first  = row_first[p];
                    int prev_second = row_second[p];

                    for (int q = 0; q < NUM_ROWS; ++q) {
                        /* Row mistake: previous row > current row lexicographically? */
                        int addv = ((prev_first > row_first[q]) ||
                                    (prev_first == row_first[q] && prev_second > row_second[q]))
                                   ? 1 : 0;
                        int new_v = v + addv;
                        if (new_v > MAX_MISTAKES) continue;

                        int new_s;
                        if (s == 0) { /* still equal so far */
                            int cur_first = row_first[q];
                            int cur_second = row_second[q];
                            if (cur_first > cur_second) new_s = 1;
                            else if (cur_first < cur_second) new_s = 2;
                            else new_s = 0;
                        } else {
                            new_s = s; /* status already decided */
                        }

                        next[new_v][new_s][q] += cur_val;
                    }
                }
            }
        }

        /* Move next to cur */
        for (int v = 0; v <= MAX_MISTAKES; ++v)
            for (int s = 0; s < NUM_STATUS; ++s)
                for (int p = 0; p < NUM_ROWS; ++p)
                    cur[v][s][p] = next[v][s][p];
    }

    /* Count arrays with exactly three mistakes */
    seq_t result = (seq_t)0;
    for (int v = 0; v <= MAX_MISTAKES; ++v) {
        for (int s = 0; s < NUM_STATUS; ++s) {
            int total_mistakes = v + (s == 2 ? 1 : 0);
            if (total_mistakes != 3) continue;
            for (int p = 0; p < NUM_ROWS; ++p)
                result += cur[v][s][p];
        }
    }
    return result;
}

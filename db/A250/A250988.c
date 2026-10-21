/*
 * A250988  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implements sequence A250988.
 *
 * Counts (n+1)×3 arrays with entries 0..2 such that:
 *   1. For each column j=1,2 the value min(x(i,j),x(i,j-1))
 *      is nondecreasing as i increases.
 *   2. For each row i the sequence of absolute differences
 *      |x(i,j)-x(i-1,j)| (for j=0,1,2) is nondecreasing as j increases.
 *
 * The computation uses dynamic programming over the rows.  The state for a
 * partial array is `(row-code, min1, min2)` where:
 *   * `row-code` is the 27‑to‑1 encoding of the current 3‑digit row.
 *   * `min1` is min(row[1],row[0]) of the current row.
 *   * `min2` is min(row[2],row[1]) of the current row.
 *
 * Complexity:   O(n * 27 * 243) operations, well below the required limit.
 * Memory usage:  two arrays of 243 `seq_t`.
 *
 * No external libraries are required, apart from the standard headers.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External integer types supplied by the build system */
#ifndef seq_t
/* The build will define seq_t appropriately */
#endif
#ifndef idx_t
/* The build will define idx_t appropriately */
#endif

/* Utility constants and precomputation for rows and differences */
#define ROWS          27          /* 3^3 possible rows */
#define MIN_STATES    3           /* min value domain {0,1,2} */
#define STATES        (ROWS * MIN_STATES * MIN_STATES) /* 27*3*3 = 243 */

/* Encode a 3‑digit row (values 0..2) as a base‑3 integer 0..26. */
static inline int max3(int a, int b, int c) {
    return a > (b > c ? b : c) ? a : (b > c ? b : c);
}
static inline int min3(int a, int b, int c) {
    return a < (b < c ? b : c) ? a : (b < c ? b : c);
}

/* Precomputed data for each row: digits and min values */
struct precomp_data {
    int row[ROWS][3];          /* digits of the row            */
    int min1[ROWS];            /* min(row[1],row[0])           */
    int min2[ROWS];            /* min(row[2],row[1])           */
    bool diff_ok[ROWS][ROWS];  /* adjacency condition          */
    bool initialized;
} PD = { {0}, {0}, {0}, {{false}}, false };

/* Initialise the lookup tables.  Called once at first use. */
static void init_precomp(void) {
    if (PD.initialized) return;

    for (int r = 0; r < ROWS; ++r) {
        int a = (r / 9) % 3;
        int b = (r / 3) % 3;
        int c = r % 3;
        PD.row[r][0] = a;
        PD.row[r][1] = b;
        PD.row[r][2] = c;
        PD.min1[r]   = (a < b) ? a : b;
        PD.min2[r]   = (b < c) ? b : c;
    }

    for (int pr = 0; pr < ROWS; ++pr) {
        for (int cr = 0; cr < ROWS; ++cr) {
            int d0 = abs(PD.row[cr][0] - PD.row[pr][0]);
            int d1 = abs(PD.row[cr][1] - PD.row[pr][1]);
            int d2 = abs(PD.row[cr][2] - PD.row[pr][2]);
            PD.diff_ok[pr][cr] = (d0 <= d1) && (d1 <= d2);
        }
    }
    PD.initialized = true;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* sequence defined for n≥1 */

    init_precomp();

    /* DP arrays: dp_prev and dp_next each have STATES entries. */
    static seq_t dp_prev[STATES];
    static seq_t dp_next[STATES];

    /* Reset dp_prev to compute from scratch. */
    for (int i = 0; i < STATES; ++i) dp_prev[i] = 0;

    /* Initial row (i = 0).  For each possible first row we record
     * the correct min1 and min2 in the state. */
    for (int r = 0; r < ROWS; ++r) {
        int idx = r * MIN_STATES * MIN_STATES
                + PD.min1[r] * MIN_STATES
                + PD.min2[r];
        dp_prev[idx] = 1;
    }

    /* Process the remaining n rows (total rows = n+1). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cur_cnt = dp_prev[s];
            if (cur_cnt == 0) continue;

            /* Decode the previous state. */
            int prev_row  = s / (MIN_STATES * MIN_STATES);
            int rem       = s % (MIN_STATES * MIN_STATES);
            int minPrev1  = rem / MIN_STATES;   /* this is previous min at j=1 */
            int minPrev2  = rem % MIN_STATES;   /* this is previous min at j=2 */

            /* Try all possible current rows. */
            for (int r = 0; r < ROWS; ++r) {
                /* Adjacency (abs difference) condition. */
                if (!PD.diff_ok[prev_row][r]) continue;

                int m1 = PD.min1[r];
                int m2 = PD.min2[r];

                /* Min monotonicity condition. */
                if (m1 < minPrev1 || m2 < minPrev2) continue;

                int idx_next = r * MIN_STATES * MIN_STATES
                              + m1 * MIN_STATES
                              + m2;
                dp_next[idx_next] += cur_cnt;
            }
        }

        /* Prepare for next iteration. */
        for (int i = 0; i < STATES; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum over all final states. */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp_prev[i];
    return total;
}

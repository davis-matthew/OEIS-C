/*
 * A251986  Number of (n+2)X(2+2) 0..2 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 1 or 5.
 * https://oeis.org/A251986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251986: Number of (n+2)×4 0..2 arrays with no 3×3 block row, column,
 *  diagonal or antidiagonal summing to 1 or 5.
 *  Implementation uses dynamic programming over consecutive rows.
 *  The solution is O(n) with a constant factor of about 60 000 operations
 *  per increase of n, well within the time limits.  */
#include <stddef.h>
#include <stdbool.h>

#define ROWS   81          /* 3^4 possible rows of length 4 */
#define STATES (ROWS * ROWS)/* pair of consecutive rows */

/* Pre‑computed digits of each row (base‑3 representation). */
static uint8_t row_digits[ROWS][4];
/* For each state (pair of rows) the number of admissible next rows,
 * and the list of those next rows. */
static int next_count[STATES];
static int next_rows[STATES][ROWS];
/* Initialization flag. */
static bool init_done = false;

/* Test whether rows r1,r2,r3 are compatible:
 * every 3×3 subblock that appears in the 3×4 block has
 * row, column, main diagonal, and antidiagonal sums neither 1 nor 5. */
static inline bool triple_ok(int r1, int r2, int r3)
{
    const uint8_t *a = row_digits[r1];
    const uint8_t *b = row_digits[r2];
    const uint8_t *c = row_digits[r3];

    for (int s = 0; s <= 1; ++s) {
        /* Row sums of the 3×3 block. */
        if (  (a[s] + a[s+1] + a[s+2]) == 1 ||
              (a[s] + a[s+1] + a[s+2]) == 5) return false;
        if (  (b[s] + b[s+1] + b[s+2]) == 1 ||
              (b[s] + b[s+1] + b[s+2]) == 5) return false;
        if (  (c[s] + c[s+1] + c[s+2]) == 1 ||
              (c[s] + c[s+1] + c[s+2]) == 5) return false;

        /* Column sums of the 3×3 block. */
        if (  (a[s]   + b[s]   + c[s])   == 1 ||
              (a[s]   + b[s]   + c[s])   == 5) return false;
        if (  (a[s+1] + b[s+1] + c[s+1]) == 1 ||
              (a[s+1] + b[s+1] + c[s+1]) == 5) return false;
        if (  (a[s+2] + b[s+2] + c[s+2]) == 1 ||
              (a[s+2] + b[s+2] + c[s+2]) == 5) return false;

        /* Main diagonal of the 3×3 block. */
        int d = a[s]   + b[s+1] + c[s+2];
        if (d == 1 || d == 5) return false;

        /* Antidiagonal of the 3×3 block. */
        d = a[s+2] + b[s+1] + c[s];
        if (d == 1 || d == 5) return false;
    }
    return true;
}

/* Build the arrays of rows and the transition tables. */
static void init(void)
{
    /* All rows in base‑3. */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int pos = 0; pos < 4; ++pos) {
            row_digits[r][pos] = x % 3;
            x /= 3;
        }
    }

    /* Transition tables. */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            int idx = r1 * ROWS + r2;
            int out = 0;
            for (int r3 = 0; r3 < ROWS; ++r3) {
                if (triple_ok(r1, r2, r3))
                    next_rows[idx][out++] = r3;
            }
            next_count[idx] = out;
        }
    }
    init_done = true;
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init();

    /* Height of the array: n+2 rows.  Width is always 4. */
    idx_t h = n + 2;

    /* DP arrays: dp_prev[state] = number of ways to reach this pair
     * of the last two rows after processing the current number of rows. */
    seq_t dp0[STATES];
    seq_t dp1[STATES];
    seq_t *dp_prev = dp0;
    seq_t *dp_curr = dp1;

    /* Start with two rows: every pair is possible.  */
    for (size_t i = 0; i < (size_t)STATES; ++i)
        dp_prev[i] = 1;

    /* Add rows one at a time until we have h rows. */
    for (idx_t len = 3; len <= h; ++len) {
        /* Reset the current DP array. */
        for (size_t i = 0; i < (size_t)STATES; ++i)
            dp_curr[i] = 0;

        /* Transition from each state. */
        for (size_t idx = 0; idx < (size_t)STATES; ++idx) {
            seq_t ways = dp_prev[idx];
            if (ways == 0) continue;

            int r1 = (int)(idx / ROWS);
            int r2 = (int)(idx % ROWS);
            int out = next_count[idx];

            for (int k = 0; k < out; ++k) {
                int r3 = next_rows[idx][k];
                size_t dest = (size_t)r2 * ROWS + r3;
                dp_curr[dest] += ways;
            }
        }

        /* Swap the DP arrays for the next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum over all states after processing all rows. */
    seq_t total = 0;
    for (size_t i = 0; i < (size_t)STATES; ++i)
        total += dp_prev[i];

    return total;
}

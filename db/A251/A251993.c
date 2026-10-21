/*
 * A251993  Number of (n+2)X(1+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 2 4 5 or 7.
 * https://oeis.org/A251993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * DP over the last two rows of a (n+2)x3 array of digits 0..3.
 * Each state encodes two consecutive rows (row i-2, row i-1) as
 * a 12‑bit integer (4^3 = 64 possible rows).  For a given pair of
 * rows (r1,r2) we precompute all rows r3 that can follow them such
 * that in the 3x3 block consisting of r1,r2,r3 every row, column,
 * diagonal and antidiagonal sum is allowed (i.e. not 2,4,5 or 7).
 *
 * The sequence a(n) is the number of (n+2)x3 arrays satisfying all
 * constraints.  The implementation works for any seq_t type.
 * It runs in O(n*4096) time and on O(4096) space.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Number of possible rows: 4^3 = 64 */
#define ROWS 64
/* Number of state pairs: 64*64 = 4096 */
#define STATES (ROWS * ROWS)

/* Tables used by the DP */
static unsigned char rows[ROWS][3];
static uint64_t allowed[STATES];
static bool forbid[10];
static bool init_done = false;

/* Build lookup tables once */
static void init_tables(void)
{
    if (init_done)
        return;

    /* Encode each row as base‑4 digits (three cells) */
    for (int r = 0; r < ROWS; ++r) {
        rows[r][0] = (r >> 4) & 3;   /* most significant cell */
        rows[r][1] = (r >> 2) & 3;   /* middle cell */
        rows[r][2] = r & 3;          /* least significant cell */
    }

    /* Forbidden sums for a 3‑cell line */
    for (int i = 0; i < 10; ++i)
        forbid[i] = false;
    forbid[2] = forbid[4] = forbid[5] = forbid[7] = true;

    /* Precompute allowed transitions: for each pair (r1,r2),
       find all r3 such that all row/column/diagonal sums are allowed. */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            uint64_t mask = 0;
            for (int r3 = 0; r3 < ROWS; ++r3) {
                /* Row sums */
                int s1 = rows[r1][0] + rows[r1][1] + rows[r1][2];
                int s2 = rows[r2][0] + rows[r2][1] + rows[r2][2];
                int s3 = rows[r3][0] + rows[r3][1] + rows[r3][2];
                if (forbid[s1] || forbid[s2] || forbid[s3])
                    continue;

                /* Column sums */
                int c1 = rows[r1][0] + rows[r2][0] + rows[r3][0];
                int c2 = rows[r1][1] + rows[r2][1] + rows[r3][1];
                int c3 = rows[r1][2] + rows[r2][2] + rows[r3][2];
                if (forbid[c1] || forbid[c2] || forbid[c3])
                    continue;

                /* Diagonals */
                int d1 = rows[r1][0] + rows[r2][1] + rows[r3][2];
                int d2 = rows[r1][2] + rows[r2][1] + rows[r3][0];
                if (forbid[d1] || forbid[d2])
                    continue;

                mask |= (1ULL << r3);
            }
            allowed[(r1 << 6) | r2] = mask;
        }
    }

    init_done = true;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* n is 1‑based; we need an array of (n+2) rows */
    init_tables();                         /* ensure tables ready */

    size_t rows_needed = (size_t)n + 2;    /* number of rows in the array */

    /* Allocate DP arrays: one entry per state (pair of rows) */
    seq_t *dp_cur   = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next  = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur); free(dp_next);
        return (seq_t)0;                   /* allocation failure */
    }

    /* Base: any two initial rows */
    for (size_t state = 0; state < STATES; ++state)
        dp_cur[state] = 1;

    /* Add remaining rows one by one */
    for (size_t len = 3; len <= rows_needed; ++len) {
        memset(dp_next, 0, STATES * sizeof(seq_t));

        for (size_t state = 0; state < STATES; ++state) {
            seq_t cur = dp_cur[state];
            if (cur == 0)
                continue;

            uint64_t mask = allowed[state];
            if (mask == 0)
                continue;

            size_t r2 = state & 0x3F;   /* second row of the pair */

            while (mask) {
                int r3 = __builtin_ctzll(mask);
                mask &= mask - 1;        /* clear lowest set bit */
                size_t next_state = (r2 << 6) | r3;
                dp_next[next_state] += cur;
            }
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum all counts – each is a distinct (n+2)x3 array satisfying the constraints */
    seq_t result = 0;
    for (size_t state = 0; state < STATES; ++state)
        result += dp_cur[state];

    free(dp_cur);
    free(dp_next);
    return result;
}

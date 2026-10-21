/*
 * A255079  Number of (n+2)X(5+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum one and no antidiagonal sum one.
 * https://oeis.org/A255079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×(5+2) 0/1 arrays with no 3×3 subblock *
 *            diagonal sum one and no antidiagonal sum one. * * Computation by
 *            dynamic programming over rows: each row is a 7‑bit * mask.
 *            States are ordered pairs (previous‑previous row, previous row).
 *            * Transitions are allowed iff the triple of ro...
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>


#define R 128                     /* number of possible 7‑bit rows */
#define STATES (R * R)            /* number of DP states */

/* decode a state index into its two row components */
static unsigned char idx_a[STATES];
static unsigned char idx_b[STATES];

/* for each pair (a,b) store the list of admissible next rows c */
static unsigned char next_len[R][R];
static unsigned char next_rows[R][R][R];

/* DP arrays – one row for current counts, one for next iteration */
static seq_t dp[STATES];
static seq_t newdp[STATES];

/* Check whether a triple of rows satisfies the two 3×3 constraints. */
static bool triple_ok(unsigned char a, unsigned char b, unsigned char c)
{
    /* iterate over the five possible starting columns of a 3×3 block */
    for (int col = 0; col < 5; ++col) {
        /* main diagonal sum */
        int diag1 =
            ((a >> col)     & 1) +
            ((b >> (col+1)) & 1) +
            ((c >> (col+2)) & 1);
        if (diag1 == 1) return false;

        /* antidiagonal sum */
        int diag2 =
            ((a >> (col+2)) & 1) +
            ((b >> (col+1)) & 1) +
            ((c >> col)     & 1);
        if (diag2 == 1) return false;
    }
    return true;
}

/* Initialise static tables once – called lazily from a(). */
static void init(bool *once)
{
    if (*once) return;
    *once = true;

    /* Build decoding tables. */
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            int id = a * R + b;
            idx_a[id] = (unsigned char)a;
            idx_b[id] = (unsigned char)b;
        }
    }

    /* Pre‑compute admissible next rows for every pair (a,b). */
    for (int a = 0; a < R; ++a) {
        for (int b = 0; b < R; ++b) {
            unsigned char *list = next_rows[a][b];
            unsigned char len = 0;
            for (int c = 0; c < R; ++c) {
                if (triple_ok((unsigned char)a, (unsigned char)b, (unsigned char)c))
                    list[len++] = (unsigned char)c;
            }
            next_len[a][b] = len;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool once = false;
    init(&once);

    /* Total number of rows in the matrix. */
    idx_t rows = n + (idx_t)2;         /* (n+2) */
    idx_t steps = rows - 2;            /* transitions required after first two rows */

    /* Initialize DP counts: every pair of first two rows is allowed. */
    for (size_t i = 0; i < STATES; ++i) {
        dp[i] = 1;
        newdp[i] = 0;
    }

    /* Perform DP over the remaining rows. */
    for (idx_t step = 0; step < steps; ++step) {
        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            unsigned char a = idx_a[idx];
            unsigned char b = idx_b[idx];
            unsigned char *c_list = next_rows[a][b];
            unsigned char len = next_len[a][b];
            for (unsigned char t = 0; t < len; ++t) {
                unsigned char c = c_list[t];
                size_t dst = (size_t)b * R + c;
                newdp[dst] += val;
            }
        }
        /* Swap dp and newdp, resetting newdp for the next iteration. */
        for (size_t i = 0; i < STATES; ++i) {
            dp[i] = newdp[i];
            newdp[i] = 0;
        }
    }

    /* Sum all ending states to obtain the total number of arrays. */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += dp[i];
    return total;
}

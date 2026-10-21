/*
 * A253726  Number of (n+1)X(2+1) 0..2 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum unequal to its neighbors
 *          horizontally and vertically.
 * https://oeis.org/A253726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A253726: number of (n+1)×3 arrays with entries 0..2
   such that for every 2×2 subblock the difference between the
   maximum of the main diagonal and the maximum of the antidiagonal
   is distinct from that of its horizontal and vertical neighbours.
   The function uses a DP over rows.  State: for the most recent pair of
   rows we keep the two diff values (for columns 0–1 and 1–2).
   Initial state: pair of rows 0 and 1, only horizontal restriction.
   Transition: add a new row, ensuring new diff values differ from each
   other (horizontal) and from the stored diff values of the same
   column (vertical). */

#include <stdbool.h>
#include <stdint.h>

/* Compile‑time constants for the DP. */
#define ROWS 27      /* 3³ possible rows (digits 0,1,2) */
#define DIFFS 5      /* differences in [-2,2], shifted by +2 */

/* Function required by the framework. */
seq_t a(idx_t n)
{
    /* Pre‑computed tables, initialized once. */
    static int rowDigits[ROWS][3];
    static signed char diff0Tab[ROWS][ROWS];
    static signed char diff1Tab[ROWS][ROWS];
    static bool initialized = false;

    if (!initialized) {
        /* Encode each row as a base‑3 number. */
        for (idx_t r = 0; r < ROWS; ++r) {
            idx_t v = r;
            for (idx_t c = 0; c < 3; ++c) {
                rowDigits[r][c] = (int)(v % 3);
                v /= 3;
            }
        }

        /* For every ordered pair of rows compute the two block diffs. */
        for (idx_t p = 0; p < ROWS; ++p) {
            for (idx_t q = 0; q < ROWS; ++q) {
                /* Diff for block columns (0,1). */
                int a0 = rowDigits[p][0];
                int b0 = rowDigits[q][1];
                int a1 = rowDigits[p][1];
                int b1 = rowDigits[q][0];
                int d0 = (a0 > b0 ? a0 : b0) - (a1 > b1 ? a1 : b1);
                diff0Tab[p][q] = (signed char)d0;

                /* Diff for block columns (1,2). */
                int a2 = rowDigits[p][1];
                int b2 = rowDigits[q][2];
                int a3 = rowDigits[p][2];
                int b3 = rowDigits[q][1];
                int d1 = (a2 > b2 ? a2 : b2) - (a3 > b3 ? a3 : b3);
                diff1Tab[p][q] = (signed char)d1;
            }
        }
        initialized = true;
    }

    /* DP tables: dp[row][d0+2][d1+2] – number of ways up to the
       current pair of rows, where d0,d1 are the diffs of the two
       blocks of the pair. */
    seq_t dp[ROWS][DIFFS][DIFFS] = {0};
    seq_t newdp[ROWS][DIFFS][DIFFS] = {0};

    /* Initialise with the single pair of first two rows (rows 0 and 1). */
    for (idx_t prev = 0; prev < ROWS; ++prev) {
        for (idx_t curr = 0; curr < ROWS; ++curr) {
            int d0 = diff0Tab[prev][curr];
            int d1 = diff1Tab[prev][curr];
            if (d0 != d1) {  /* horizontal rule only for n = 1 */
                dp[curr][d0 + 2][d1 + 2] += (seq_t)1;
            }
        }
    }

    if (n == 1) {  /* Only two rows – sum all valid states. */
        seq_t total = 0;
        for (idx_t r = 0; r < ROWS; ++r)
            for (int d0i = 0; d0i < DIFFS; ++d0i)
                for (int d1i = 0; d1i < DIFFS; ++d1i)
                    total += dp[r][d0i][d1i];
        return total;
    }

    /* Add rows 2 … n. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero newdp. */
        for (idx_t r = 0; r < ROWS; ++r)
            for (int d0i = 0; d0i < DIFFS; ++d0i)
                for (int d1i = 0; d1i < DIFFS; ++d1i)
                    newdp[r][d0i][d1i] = 0;

        for (idx_t prevRow = 0; prevRow < ROWS; ++prevRow) {
            for (int d0i = 0; d0i < DIFFS; ++d0i) {
                for (int d1i = 0; d1i < DIFFS; ++d1i) {
                    seq_t count = dp[prevRow][d0i][d1i];
                    if (count == 0) continue;
                    int d0prev = d0i - 2;   /* previous diff for left block */
                    int d1prev = d1i - 2;   /* previous diff for right block */
                    for (idx_t currRow = 0; currRow < ROWS; ++currRow) {
                        int d0cur = diff0Tab[prevRow][currRow];
                        int d1cur = diff1Tab[prevRow][currRow];
                        if (d0cur == d1cur) continue;          /* horizontal */
                        if (d0cur == d0prev) continue;         /* vertical left */
                        if (d1cur == d1prev) continue;         /* vertical right */
                        newdp[currRow][d0cur + 2][d1cur + 2] += count;
                    }
                }
            }
        }

        /* Move newdp into dp for the next iteration. */
        for (idx_t r = 0; r < ROWS; ++r)
            for (int d0i = 0; d0i < DIFFS; ++d0i)
                for (int d1i = 0; d1i < DIFFS; ++d1i)
                    dp[r][d0i][d1i] = newdp[r][d0i][d1i];
    }

    /* Sum over all ending states to obtain the answer. */
    seq_t result = 0;
    for (idx_t r = 0; r < ROWS; ++r)
        for (int d0i = 0; d0i < DIFFS; ++d0i)
            for (int d1i = 0; d1i < DIFFS; ++d1i)
                result += dp[r][d0i][d1i];

    return result;
}

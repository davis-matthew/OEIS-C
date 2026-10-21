/*
 * A278729  Number of nX3 0..3 arrays with rows in nondecreasing lexicographic
 *          order and columns in nonincreasing lexicographic order, but with
 *          exactly one mistake.
 * https://oeis.org/A278729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays over {0,1,2,3} whose rows * are in
 *            nondecreasing lexicographic order and whose columns are in *
 *            nonincreasing lexicographic order, * having exactly one
 *            violation of these ordering constraints
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 16, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define ROWS      64   /* 4^3 possible rows */
#define STATUS    3    /* 0 = undecided, 1 = good, 2 = bad */
#define MIST_CODES 2   /* 0 or 1 mistakes so far */

static unsigned char row_digits[ROWS][3];
static bool digits_precomputed = false;

/* Compare two rows lexicographically.
 * Return 1 if first row > second row, otherwise 0. */
static int row_mistake(int r1, int r2)
{
    unsigned char d1 = row_digits[r1][0];
    unsigned char d2 = row_digits[r2][0];
    if (d1 > d2) return 1;
    if (d1 < d2) return 0;
    d1 = row_digits[r1][1];
    d2 = row_digits[r2][1];
    if (d1 > d2) return 1;
    if (d1 < d2) return 0;
    d1 = row_digits[r1][2];
    d2 = row_digits[r2][2];
    return (d1 > d2) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence offset is 1 */

    if (!digits_precomputed) {
        for (int r = 0; r < ROWS; ++r) {
            row_digits[r][0] = (unsigned char)(r / 16);
            row_digits[r][1] = (unsigned char)((r / 4) % 4);
            row_digits[r][2] = (unsigned char)(r % 4);
        }
        digits_precomputed = true;
    }

    static seq_t dp_cur[ROWS][STATUS][STATUS][MIST_CODES];
    static seq_t dp_next[ROWS][STATUS][STATUS][MIST_CODES];
    /* clear current DP */
    for (int r = 0; r < ROWS; ++r)
        for (int s1 = 0; s1 < STATUS; ++s1)
            for (int s2 = 0; s2 < STATUS; ++s2)
                for (int m = 0; m < MIST_CODES; ++m)
                    dp_cur[r][s1][s2][m] = (seq_t)0;

    /* Initial row: no previous row, but column statuses depend on the
     * first row itself. */
    for (int r = 0; r < ROWS; ++r) {
        unsigned char a0 = row_digits[r][0];
        unsigned char a1 = row_digits[r][1];
        unsigned char a2 = row_digits[r][2];
        int s01 = (a0 > a1) ? 1 : (a0 < a1) ? 2 : 0;
        int s12 = (a1 > a2) ? 1 : (a1 < a2) ? 2 : 0;
        int mist = (s01 == 2) + (s12 == 2);
        if (mist <= 1) {
            dp_cur[r][s01][s12][mist] = dp_cur[r][s01][s12][mist] + (seq_t)1;
        }
    }

    /* Process remaining rows (n-1) */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next DP */
        for (int r = 0; r < ROWS; ++r)
            for (int s1 = 0; s1 < STATUS; ++s1)
                for (int s2 = 0; s2 < STATUS; ++s2)
                    for (int m = 0; m < MIST_CODES; ++m)
                        dp_next[r][s1][s2][m] = (seq_t)0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int s01 = 0; s01 < STATUS; ++s01) {
                for (int s12 = 0; s12 < STATUS; ++s12) {
                    for (int m0 = 0; m0 < MIST_CODES; ++m0) {
                        seq_t cnt = dp_cur[prev][s01][s12][m0];
                        if (cnt == (seq_t)0) continue;

                        for (int next = 0; next < ROWS; ++next) {
                            /* Row order mistake? */
                            int rmist = row_mistake(prev, next);

                            /* Column status updates */
                            unsigned char c0 = row_digits[next][0];
                            unsigned char c1 = row_digits[next][1];
                            unsigned char c2 = row_digits[next][2];

                            int ns01 = s01;
                            int ns12 = s12;
                            if (s01 == 0) {
                                if (c0 > c1) ns01 = 1;
                                else if (c0 < c1) ns01 = 2;
                            }
                            if (s12 == 0) {
                                if (c1 > c2) ns12 = 1;
                                else if (c1 < c2) ns12 = 2;
                            }

                            int incMist = 0;
                            if (rmist) incMist++;
                            if (s01 == 0 && c0 < c1) incMist++;
                            if (s12 == 0 && c1 < c2) incMist++;

                            int mnew = m0 + incMist;
                            if (mnew > 1) continue;   /* only allow at most one mistake */

                            dp_next[next][ns01][ns12][mnew] =
                                dp_next[next][ns01][ns12][mnew] + cnt;
                        }
                    }
                }
            }
        }

        /* copy dp_next back to dp_cur for next iteration */
        for (int r = 0; r < ROWS; ++r)
            for (int s1 = 0; s1 < STATUS; ++s1)
                for (int s2 = 0; s2 < STATUS; ++s2)
                    for (int m = 0; m < MIST_CODES; ++m)
                        dp_cur[r][s1][s2][m] = dp_next[r][s1][s2][m];
    }

    /* Sum over states with exactly one mistake */
    seq_t result = (seq_t)0;
    for (int r = 0; r < ROWS; ++r)
        for (int s01 = 0; s01 < STATUS; ++s01)
            for (int s12 = 0; s12 < STATUS; ++s12)
                result = result + dp_cur[r][s01][s12][1];

    return result;
}

/*
 * A297015  Number of nX3 0..1 arrays with each 1 adjacent to 3, 4 or 6
 *          king-move neighboring 1s.
 * https://oeis.org/A297015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A297015: Number of n×3 0/1 arrays where each 1 has
 * 3, 4, or 6 king‑move neighboring 1s.
 *
 * Computation by dynamic programming over all 3‑bit row patterns.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to read a bit from a 3‑bit pattern. */
static inline int bit(int p, int j) { return (p >> j) & 1; }

/* Precompute which triples of rows (prev, curr, next) keep all 1s of curr
 * with 3, 4 or 6 adjacent 1s. */
static bool valid[8][8][8];
static bool valid_inited = false;
static void
init_valid(void)
{
    if (valid_inited) return;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int next = 0; next < 8; ++next) {
                bool ok = true;
                for (int j = 0; j < 3 && ok; ++j) {
                    if (!bit(curr, j)) continue;          /* only 1s matter */
                    int cnt = 0;
                    /* current row horizontal neighbors */
                    if (j > 0 && bit(curr, j - 1)) ++cnt;
                    if (j < 2 && bit(curr, j + 1)) ++cnt;
                    /* above row */
                    if (j > 0 && bit(prev, j - 1)) ++cnt;
                    if (bit(prev, j))     ++cnt;
                    if (j < 2 && bit(prev, j + 1)) ++cnt;
                    /* below row */
                    if (j > 0 && bit(next, j - 1)) ++cnt;
                    if (bit(next, j))     ++cnt;
                    if (j < 2 && bit(next, j + 1)) ++cnt;
                    if (cnt != 3 && cnt != 4 && cnt != 6) ok = false;
                }
                valid[prev][curr][next] = ok;
            }
        }
    }
    valid_inited = true;
}

/* DP arrays: dp[8][8] holds counts for last two rows (prev, curr). */
static seq_t dp[8][8];
static bool dp_inited = false;

/* Store previously computed a(n) values.  answers[0] is unused. */
static seq_t *answers = NULL;
static size_t answer_count = 0;   /* largest n computed (>=1) */

/* Main sequence function. */
seq_t
a(idx_t n)
{
    init_valid();

    /* Allocate or extend answers array to hold up to n. */
    if (!answers) {
        answers = (seq_t*)malloc(((size_t)n + 1) * sizeof(seq_t));
    } else if (answer_count < n) {
        seq_t *tmp = (seq_t*)realloc(answers, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        answers = tmp;
    }

    /* Initialise DP for n = 1 if we have not. */
    if (!dp_inited) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = 0;
        for (int r = 0; r < 8; ++r)
            dp[0][r] = (seq_t)1;          /* row 0 is all‑zero, row 1 = r */
        seq_t sum = 0;
        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr)
                if (valid[prev][curr][0]) sum += dp[prev][curr];
        answers[1] = sum;
        answer_count = 1;
        dp_inited = true;
    }

    /* Extend DP until answer_count == n. */
    while (answer_count < n) {
        /* newdp will hold counts for rows processed so far (i+1) */
        seq_t newdp[8][8];
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < 8; ++next) {
                    if (valid[prev][curr][next]) {
                        newdp[curr][next] += ways;
                    }
                }
            }
        }

        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];

        /* compute answer for (answer_count+1) rows */
        seq_t sum = 0;
        for (int prev = 0; prev < 8; ++prev)
            for (int curr = 0; curr < 8; ++curr)
                if (valid[prev][curr][0]) sum += dp[prev][curr];

        answers[++answer_count] = sum;
    }

    return answers[n];
}

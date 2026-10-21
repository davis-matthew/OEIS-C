/*
 * A269933  Number of nX3 0..6 arrays with some element plus some
 *          horizontally, diagonally, antidiagonally or vertically adjacent
 *          neighbor totalling six exactly once.
 * https://oeis.org/A269933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computed by dynamic programming over rows; transitions *
 *            consider horizontal, vertical, diagonal and antidiagonal pairs.
 *            * Counts n×3 arrays over 0..6 that contain EXACTLY one pair of
 *            adjacent * cells summing to 6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define NUM_ROWS 343  /* 7^3 possible rows */

/* ------------------------------------------------------------------ */
/*  a(idx_t n) : sequence value for index n (1‑based, n>=1)             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0 */

    /* ------------------------------------------------------------------
     * Pre‑computed data (initialized once)
     * ------------------------------------------------------------------ */
    static int h[NUM_ROWS];                     /* horizontal pair count per row   */
    static unsigned char dig[NUM_ROWS][3];      /* digits of each row              */
    static unsigned char cross[NUM_ROWS][NUM_ROWS]; /* cross‑row pair counts   */
    static int *zero_list[NUM_ROWS];           /* next‑row indices giving d==0    */
    static int *one_list[NUM_ROWS];            /* next‑row indices giving d==1    */
    static int zero_len[NUM_ROWS];
    static int one_len[NUM_ROWS];

    static seq_t *dp0 = NULL, *dp1 = NULL;      /* current DP states            */
    static seq_t *ndp0 = NULL, *ndp1 = NULL;    /* next DP states               */
    static seq_t *answers = NULL;               /* memoised answer table        */
    static idx_t ans_cap = 0, max_cached = 0;
    static int initialized = 0;

    if (!initialized) {
        /* digits of all rows and horizontal pair count */
        for (int r = 0; r < NUM_ROWS; ++r) {
            int x = r;
            dig[r][0] = x / 49;
            x %= 49;
            dig[r][1] = x / 7;
            dig[r][2] = x % 7;
            h[r] = ((dig[r][0] + dig[r][1] == 6) ? 1 : 0) +
                   ((dig[r][1] + dig[r][2] == 6) ? 1 : 0);
        }

        /* cross‑row pair matrix */
        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
                int s = 0;
                for (int j = 0; j < 3; ++j) {
                    if (dig[r1][j] + dig[r2][j] == 6)   ++s;          /* vertical   */
                    if (j > 0 && dig[r1][j] + dig[r2][j-1] == 6) ++s;  /* antidiag  */
                    if (j < 2 && dig[r1][j] + dig[r2][j+1] == 6) ++s;  /* diag      */
                }
                cross[r1][r2] = (unsigned char)s;
            }
        }

        /* adjacency lists for d = 0 and d = 1 */
        memset(zero_len, 0, sizeof(zero_len));
        memset(one_len, 0, sizeof(one_len));
        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
                int d = h[r2] + cross[r1][r2];
                if (d == 0) ++zero_len[r1];
                else if (d == 1) ++one_len[r1];
            }
        }

        for (int r = 0; r < NUM_ROWS; ++r) {
            zero_list[r] = zero_len[r] ? (int*)malloc(zero_len[r] * sizeof(int)) : NULL;
            one_list[r]  = one_len[r]  ? (int*)malloc(one_len[r]  * sizeof(int)) : NULL;
        }

        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            int *zp = zero_list[r1];
            int *op = one_list[r1];
            for (int r2 = 0; r2 < NUM_ROWS; ++r2) {
                int d = h[r2] + cross[r1][r2];
                if (d == 0) *zp++ = r2;
                else if (d == 1) *op++ = r2;
            }
        }

        /* DP arrays */
        dp0  = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));
        dp1  = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));
        ndp0 = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));
        ndp1 = (seq_t*)calloc(NUM_ROWS, sizeof(seq_t));

        initialized = 1;
    }

    /* ------------------------------------------------------------------
     * Ensure answers array capacity
     * ------------------------------------------------------------------ */
    if (n >= ans_cap) {
        idx_t newcap = ans_cap ? ans_cap * 2 : 16;
        while (newcap <= n) newcap <<= 1;
        answers = (seq_t*)realloc(answers, newcap * sizeof(seq_t));
        for (idx_t i = ans_cap; i < newcap; ++i) answers[i] = 0;
        ans_cap = newcap;
    }

    /* ------------------------------------------------------------------
     * Build up to the requested n (incrementally)
     * ------------------------------------------------------------------ */
    if (max_cached == 0) {   /* initialise with first row */
        for (int i = 0; i < NUM_ROWS; ++i) dp0[i] = dp1[i] = 0;
        for (int r = 0; r < NUM_ROWS; ++r) {
            if (h[r] == 0) dp0[r] = 1;
            else if (h[r] == 1) dp1[r] = 1;
        }
        seq_t sum = 0;
        for (int r = 0; r < NUM_ROWS; ++r) sum += dp1[r];
        answers[1] = sum;
        max_cached = 1;
    }

    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        /* clear new DP arrays */
        memset(ndp0, 0, NUM_ROWS * sizeof(seq_t));
        memset(ndp1, 0, NUM_ROWS * sizeof(seq_t));

        /* transitions */
        for (int r1 = 0; r1 < NUM_ROWS; ++r1) {
            seq_t c0 = dp0[r1];
            seq_t c1 = dp1[r1];
            if (c0) {
                for (int i = 0; i < zero_len[r1]; ++i) {
                    int r2 = zero_list[r1][i];
                    ndp0[r2] += c0;
                }
                for (int i = 0; i < one_len[r1]; ++i) {
                    int r2 = one_list[r1][i];
                    ndp1[r2] += c0;
                }
            }
            if (c1) {
                for (int i = 0; i < zero_len[r1]; ++i) {
                    int r2 = zero_list[r1][i];
                    ndp1[r2] += c1;
                }
            }
        }

        /* store answer for this length */
        seq_t sum = 0;
        for (int r = 0; r < NUM_ROWS; ++r) sum += ndp1[r];
        answers[cur] = sum;

        /* swap dp arrays for next step */
        seq_t *tmp = dp0;  dp0 = ndp0;  ndp0 = tmp;
        tmp = dp1;         dp1 = ndp1;  ndp1 = tmp;
    }

    max_cached = n;
    return answers[n];
}

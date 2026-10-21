/*
 * A252082  Number of (n+2)X(2+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 3 or 4.
 * https://oeis.org/A252082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252082 – Number of (n+2)×4 0..2 arrays with no 3×3 subblock
 * whose row, column, diagonal or antidiagonal sum equals 1, 3 or 4.
 *
 * Computed by dynamic programming over rows.  For each pair of
 * consecutive rows (state) and a candidate new row we test all
 * 3×3 subblocks that involve the new row.  Valid transitions are
 * pre‑computed and stored in small tables, yielding an average
 * of only a few updates per state.  The algorithm runs in
 * O(n·6561) time and uses a few megabytes of memory, easily
 * fitting the 2‑second limit for n up to 10 000.
 */

#include <stdint.h>
#include <stdlib.h>

#define ROWS            81                      /* 3^4 possible rows */
#define STATES          (ROWS * ROWS)            /* 6561 state pairs */
#define TRANS_POOL_SZ   (STATES * ROWS)          /* max possible transitions */

/* encode each 4‑cell row; row_digits[r][c] is the value in column c */
static uint8_t row_digits[ROWS][4];

/* transition tables */
static uint8_t  trans_len[STATES];      /* number of transitions for state s   */
static uint32_t trans_start[STATES];    /* index of first transition in pool */
static uint8_t  trans_pool[TRANS_POOL_SZ];

/* pre‑computation flag                                   */
static int precomp_done = 0;

/* sum is forbidden when it equals 1, 3 or 4                */
static inline int forbidden(int s)
{
    return (s == 1 || s == 3 || s == 4);
}

/* Build the row digit tables and the transition lists      */
static void init_precomp(void)
{
    /* Build digit tables for all 81 possible rows   */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int c = 3; c >= 0; --c) {
            row_digits[r][c] = x % 3;
            x /= 3;
        }
    }

    size_t pool_ptr = 0;
    for (int s = 0; s < STATES; ++s) {
        int r0 = s / ROWS;          /* old previous row  */
        int r1 = s % ROWS;          /* old last row      */
        trans_start[s] = (uint32_t)pool_ptr;
        uint8_t count = 0;

        for (int r2 = 0; r2 < ROWS; ++r2) {      /* candidate new row */
            int ok = 1;

            /* Check both possible 3×3 subblocks that span these three rows */
            for (int cs = 0; cs < 2 && ok; ++cs) {   /* column start (0 or 1) */
                /* Row sums */
                int sum;
                sum = row_digits[r0][cs] + row_digits[r0][cs+1] + row_digits[r0][cs+2];
                if (forbidden(sum)) { ok = 0; break; }
                sum = row_digits[r1][cs] + row_digits[r1][cs+1] + row_digits[r1][cs+2];
                if (forbidden(sum)) { ok = 0; break; }
                sum = row_digits[r2][cs] + row_digits[r2][cs+1] + row_digits[r2][cs+2];
                if (forbidden(sum)) { ok = 0; break; }

                /* Column sums */
                for (int c = 0; c < 3 && ok; ++c) {
                    sum = row_digits[r0][cs+c] + row_digits[r1][cs+c] + row_digits[r2][cs+c];
                    if (forbidden(sum)) { ok = 0; break; }
                }

                /* Main diagonal */
                if (ok) {
                    sum = row_digits[r0][cs]     + row_digits[r1][cs+1] + row_digits[r2][cs+2];
                    if (forbidden(sum)) ok = 0;
                }

                /* Anti‑diagonal */
                if (ok) {
                    sum = row_digits[r0][cs+2]   + row_digits[r1][cs+1] + row_digits[r2][cs];
                    if (forbidden(sum)) ok = 0;
                }
            }

            if (ok) {
                trans_pool[pool_ptr++] = (uint8_t)r2;
                count++;
            }
        }
        trans_len[s] = count;
    }
    precomp_done = 1;
}

/* ---------------------------------------------------------- */
/* Main OEIS sequence function                               */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!precomp_done) init_precomp();   /* pre‑computation once */

    size_t rows_needed = (size_t)n + 2;   /* (n+2) rows in total          */

    /* static DP buffers – reused between calls          */
    static seq_t *dp = NULL;
    static seq_t *newdp = NULL;
    static size_t dp_cap = 0;

    if (dp_cap < (size_t)STATES) {
        free(dp);
        free(newdp);
        dp = (seq_t*)malloc(sizeof(seq_t) * STATES);
        newdp = (seq_t*)malloc(sizeof(seq_t) * STATES);
        dp_cap = STATES;
    }

    /* initialise after the first two rows – all 6561 pairs possible one time each */
    for (size_t i = 0; i < (size_t)STATES; ++i)
        dp[i] = (seq_t)1;

    /* add rows one by one, starting with the third row            */
    for (size_t r = 2; r < rows_needed; ++r) {
        /* clear newdp */
        for (size_t i = 0; i < (size_t)STATES; ++i)
            newdp[i] = (seq_t)0;

        for (size_t s = 0; s < (size_t)STATES; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;

            uint8_t c = trans_len[s];
            uint32_t base = trans_start[s];

            for (uint8_t k = 0; k < c; ++k) {
                uint8_t r2 = trans_pool[base + k];
                size_t ns = ((s % ROWS) * ROWS) + r2;
                newdp[ns] += cnt;
            }
        }
        /* swap buffers for next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    /* sum all valid end states */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < (size_t)STATES; ++i)
        result += dp[i];

    return result;
}

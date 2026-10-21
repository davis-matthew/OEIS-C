/*
 * A251041  Number of (4+1)X(n+1) 0..3 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A251041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 19, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251041: number of (4+1)×(n+1) 0..3 arrays such that  
 *  (i) for each column j, the sequence min(x(i,j),x(i,j-1)) is nondecreasing
 *      as i increases, and
 *  (ii) for each column j, the sequence |x(i,j)-x(i-1,j)| is
 *      nondecreasing as i increases.
 *
 *  The function computes this count for any n≥1.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define NUM_ROWS   5
#define STATE_SZ   (1U << (2 * NUM_ROWS))   /* 4^5 = 1024, constant at compile time */

seq_t a(idx_t n)
{
    /* ----- Build transition graph once --------------------------------- */
    static bool built = false;
    static uint8_t rows[STATE_SZ][NUM_ROWS];      /* decoded rows per state   */
    static uint16_t neigh[STATE_SZ][STATE_SZ];   /* adjacency lists (dense)  */
    static uint16_t neigh_cnt[STATE_SZ];          /* number of neighbors      */

    if (!built) {
        /* Decode each state into its 5 row values (0..3) */
        for (uint32_t s = 0; s < STATE_SZ; ++s) {
            for (uint32_t r = 0; r < NUM_ROWS; ++r) {
                rows[s][r] = (uint8_t)((s >> (2 * r)) & 3U);
            }
        }

        /* Build the compatibility graph */
        for (uint32_t p = 0; p < STATE_SZ; ++p) {
            uint16_t *nbr = neigh[p];
            uint16_t cnt = 0;
            for (uint32_t c = 0; c < STATE_SZ; ++c) {
                bool ok = true;
                int prev_min = 0;
                int prev_diff = 0;   /* previous row difference in the same column */

                /* Row 0 */
                int min0 = rows[p][0] < rows[c][0] ? rows[p][0] : rows[c][0];
                /* For row 0, difference across rows is not defined; set prev_diff=0 */
                prev_min = min0;
                prev_diff = 0;

                /* Rows 1..NUM_ROWS-1 */
                for (int r = 1; r < NUM_ROWS; ++r) {
                    int cur_min = rows[p][r] < rows[c][r] ?
                                   rows[p][r] : rows[c][r];
                    if (cur_min < prev_min) { ok = false; break; }
                    prev_min = cur_min;

                    int prev_row_diff = abs((int)rows[p][r] - (int)rows[p][r-1]);
                    int cur_row_diff  = abs((int)rows[c][r] - (int)rows[c][r-1]);
                    if (cur_row_diff < prev_row_diff) { ok = false; break; }
                    prev_diff = cur_row_diff;
                }

                if (ok) {
                    nbr[cnt++] = (uint16_t)c;
                }
            }
            neigh_cnt[p] = cnt;
        }
        built = true;
    }

    /* ----- DP over columns ---------------------------------------------- */
    static seq_t cur[STATE_SZ];
    static seq_t nxt[STATE_SZ];

    for (uint32_t s = 0; s < STATE_SZ; ++s) cur[s] = (seq_t)1;  /* first column */

    for (uint64_t step = 1; step <= n; ++step) {
        memset(nxt, 0, sizeof(nxt));

        for (uint32_t p = 0; p < STATE_SZ; ++p) {
            seq_t val = cur[p];
            if (!val) continue;
            uint16_t *nbr = neigh[p];
            uint16_t cnt  = neigh_cnt[p];
            for (uint16_t k = 0; k < cnt; ++k) {
                uint16_t c = nbr[k];
                nxt[c] += val;
            }
        }
        memcpy(cur, nxt, sizeof(cur));
    }

    /* Sum all ending states */
    seq_t total = (seq_t)0;
    for (uint32_t s = 0; s < STATE_SZ; ++s) total += cur[s];
    return total;
}

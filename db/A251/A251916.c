/*
 * A251916  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum 2 4 5 or 7 and every diagonal and antidiagonal sum not
 *          2 4 5 or 7.
 * https://oeis.org/A251916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute a(n) by dynamic programming over rows. Each (n+2)-by-3
 *            array must have every 3x3 subblock row and column sums in
 *            {2,4,5,7} and every main or anti‑diagonal sum NOT in that set.
 *            The DP counts the number of ways to extend a pair of rows to the
 *            required height. It uses a precomputed adjacency...
 * Range:     u8 none, u16 n <= 10, u32 n <= 27, u64 n <= 60, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* ----- constants ------------------------------------------------------- */
#define N_ROWS   64U            /* 3‑digit base–4 rows 0 … 63 */
#define N_STATES (N_ROWS * N_ROWS)  /* ordered row pairs */

/* ----- precomputation data -------------------------------------------- */
static uint8_t  row_digit[N_ROWS][3];
static uint8_t  row_sum  [N_ROWS];
static bool     allowed_sum[10];

static uint8_t  adj_list[N_STATES][N_ROWS];   /* each state may list up to 64 next rows */
static uint8_t  adj_len  [N_STATES];

static seq_t   dp_buf1[N_STATES];
static seq_t   dp_buf2[N_STATES];
static seq_t  *dp_cur;
static seq_t  *dp_next;

/* ----- initialization --------------------------------------------------- */
static bool initialized = false;
static void init_precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* allowed 3‑sum values */
    for (int i = 0; i < 10; ++i) allowed_sum[i] = false;
    allowed_sum[2] = allowed_sum[4] = allowed_sum[5] = allowed_sum[7] = true;

    /* compute row digits and row sums */
    for (int r = 0; r < N_ROWS; ++r) {
        int v = r;
        row_digit[r][0] = (v >> 4) & 3;          /* most significant 2 bits */
        row_digit[r][1] = (v >> 2) & 3;
        row_digit[r][2] =  v        & 3;
        row_sum[r]      = row_digit[r][0] + row_digit[r][1] + row_digit[r][2];
    }

    /* build adjacency lists: (ra,rb) → list of rc that form a valid block */
    for (int ra = 0; ra < N_ROWS; ++ra) {
        for (int rb = 0; rb < N_ROWS; ++rb) {
            size_t idx  = (size_t)ra * N_ROWS + rb;
            uint8_t  *list = adj_list[idx];
            uint8_t   len  = 0;

            for (int rc = 0; rc < N_ROWS; ++rc) {
                /* check row sums */
                if (!allowed_sum[row_sum[ra]]) continue;
                if (!allowed_sum[row_sum[rb]]) continue;
                if (!allowed_sum[row_sum[rc]]) continue;

                /* column sums */
                int c0 = row_digit[ra][0] + row_digit[rb][0] + row_digit[rc][0];
                int c1 = row_digit[ra][1] + row_digit[rb][1] + row_digit[rc][1];
                int c2 = row_digit[ra][2] + row_digit[rb][2] + row_digit[rc][2];
                if (!allowed_sum[c0] || !allowed_sum[c1] || !allowed_sum[c2])
                    continue;

                /* diagonals must NOT be allowed */
                int d1 = row_digit[ra][0] + row_digit[rb][1] + row_digit[rc][2];
                int d2 = row_digit[ra][2] + row_digit[rb][1] + row_digit[rc][0];
                if (allowed_sum[d1] || allowed_sum[d2]) continue;

                list[len++] = (uint8_t)rc;
            }
            adj_len[idx] = len;
        }
    }

    dp_cur  = dp_buf1;
    dp_next = dp_buf2;
}

/* ----- sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) → array height h = n + 2 rows */
    idx_t h = n + 2;
    if (h < 3) return 0;      /* undefined for n < 1, but never reached */

    init_precompute();

    /* initialise dp_cur: one way for every pair of first two rows */
    for (size_t i = 0; i < N_STATES; ++i)
        dp_cur[i] = 1;

    /* extend rows from height 3 up to h */
    for (idx_t step = 3; step <= h; ++step) {
        /* clear dp_next */
        memset(dp_next, 0, sizeof(seq_t) * N_STATES);

        /* for every previous pair (ra,rb) let val be its count */
        for (size_t idx = 0; idx < N_STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (!val) continue;

            uint8_t rb = (uint8_t)(idx & (N_ROWS - 1)); /* second row of the pair */

            uint8_t *list = adj_list[idx];
            uint8_t  len  = adj_len[idx];
            for (uint8_t j = 0; j < len; ++j) {
                uint8_t rc = list[j];
                size_t  dest = (size_t)rb * N_ROWS + rc;
                dp_next[dest] += val;
            }
        }

        /* swap dp_cur and dp_next pointers */
        seq_t *tmp = dp_cur;
        dp_cur  = dp_next;
        dp_next = tmp;
    }

    /* sum all ways in the final dp_cur */
    seq_t total = 0;
    for (size_t i = 0; i < N_STATES; ++i)
        total += dp_cur[i];

    return total;
}

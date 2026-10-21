/*
 * A252450  Number of (n+2) X (1+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 2 4 6 or 7.
 * https://oeis.org/A252450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252450 – Number of (n+2)×3 arrays with digits 0..3 such that
 * every 3×3 subblock has row, column, diagonal and anti‑diagonal
 * sums not equal to 0, 2, 4, 6 or 7.
 *
 * The function computes the number by dynamic programming over the two
 * most recent rows.  For each n≥1 it performs n transitions starting
 * from the 2‑row base state.  All arithmetic is performed in seq_t,
 * which may be arbitrary‑precision or an unsigned integer type.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- Constants and global tables --------------------------------------- */
#define ROWS   64             /* 4^3 possible rows (digits 0..3)      */
#define STATES (ROWS*ROWS)    /* pair of rows as a state              */

/* Row data */
static uint8_t row_digit[ROWS][3];
static uint8_t row_sum[ROWS];

/* Forbidden sums: 0,2,4,6,7 prohibited */
static bool forbid[10];

/* Transition tables: from state (prevprev,prev) to list of valid next rows */
static uint8_t adj_next[STATES][ROWS];
static uint8_t adj_out[STATES];

/* DP arrays for counts of arrays of current length (dp_length rows) */
static seq_t dp[STATES];
static seq_t nextdp[STATES];

/* Sequence cache */
static seq_t *cache = NULL;
static idx_t cached_max = 0;     /* highest n for which cache[n] is valid */

/* Current length represented by dp (starts with 2 rows) */
static idx_t dp_length = 2;

/* -------------------------------------------------------------------- */
/* Initialise lookup tables (once) */
static void init_tables(void)
{
    /* precompute row representations and sums */
    for (uint8_t r = 0; r < ROWS; ++r) {
        row_digit[r][0] = r & 3;
        row_digit[r][1] = (r >> 2) & 3;
        row_digit[r][2] = (r >> 4) & 3;
        row_sum[r] = row_digit[r][0] + row_digit[r][1] + row_digit[r][2];
    }

    /* set forbidden sums */
    forbid[0] = forbid[2] = forbid[4] = forbid[6] = forbid[7] = true;
    for (int i = 1; i < 10; ++i)
        if (!forbid[i]) forbid[i] = false;

    /* build adjacency lists */
    for (uint8_t a = 0; a < ROWS; ++a) {
        for (uint8_t b = 0; b < ROWS; ++b) {
            uint16_t state = (a << 6) | b;
            uint8_t out = 0;
            for (uint8_t c = 0; c < ROWS; ++c) {
                /* collect the 8 relevant sums: 3 row sums, 3 column sums,
                 * 1 main diagonal, 1 anti-diagonal                      */
                uint8_t sums[8];
                sums[0] = row_sum[a];
                sums[1] = row_sum[b];
                sums[2] = row_sum[c];
                for (uint8_t col = 0; col < 3; ++col)
                    sums[3 + col] = row_digit[a][col] + row_digit[b][col] + row_digit[c][col];
                sums[6] = row_digit[a][0] + row_digit[b][1] + row_digit[c][2];
                sums[7] = row_digit[a][2] + row_digit[b][1] + row_digit[c][0];

                bool bad = false;
                for (uint8_t j = 0; j < 8; ++j)
                    if (forbid[sums[j]]) { bad = true; break; }

                if (!bad) {
                    adj_next[state][out++] = c;
                }
            }
            adj_out[state] = out;
        }
    }
}

/* -------------------------------------------------------------------- */
/* Extend DP and cache up to n (inclusive) */
static seq_t compute_all(idx_t up_to)
{
    /* initialise tables once */
    static bool tables_inited = false;
    if (!tables_inited) { init_tables(); tables_inited = true; }

    /* grow cache if necessary */
    if (!cache || cached_max < up_to) {
        idx_t old_max = cached_max;
        idx_t new_size = up_to + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* initialise DP if starting from scratch */
        if (old_max == 0) {
            for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;
            dp_length = 2;
        }

        /* extend DP and cache for new n values */
        for (idx_t n = old_max + 1; n <= up_to; ++n) {
            memset(nextdp, 0, sizeof(nextdp));

            for (uint16_t state = 0; state < STATES; ++state) {
                seq_t cnt = dp[state];
                if (!cnt) continue;
                uint8_t out = adj_out[state];
                for (uint8_t k = 0; k < out; ++k) {
                    uint8_t next_row = adj_next[state][k];
                    uint16_t new_state = ((state & 63) << 6) | next_row;
                    nextdp[new_state] += cnt;
                }
            }

            memcpy(dp, nextdp, sizeof(dp));
            ++dp_length;   /* dp now represents arrays of length dp_length rows */

            /* total count for this n (height = n+2 rows) */
            seq_t total = 0;
            for (int i = 0; i < STATES; ++i) total += dp[i];
            cache[n] = total;
        }
        cached_max = up_to;
    }
    return cache[up_to];
}

/* -------------------------------------------------------------------- */
/* Public API */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    return compute_all(n);
}

/*
 * A275897  Read the infinite chessboard underlying A065188 by successive
 *          antidiagonals and record when the queens are encountered. Here the
 *          rows and columns are indexed starting at 0 (as in A275895).
 * https://oeis.org/A275897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = employee of the n‑th queen in the greedy * infinite
 *            chessboard, read by successive antidiagonals. * The function
 *            uses a simple exhaustive search, maintaining, * for all rows,
 *            columns and both diagonals, whether a queen * already occupies
 *            them. It remembers previously found queens * so that...
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* choose a limit that comfortably covers the board indices that
 * are needed for n up to about 10000.  The arrays store 0/1
 * flags for rows, columns, main diagonals (i-j) and
 * anti‑diagonals (i+j). */
#define MAX_COORD 200000

static uint8_t row_used[MAX_COORD + 1];
static uint8_t col_used[MAX_COORD + 1];
static uint8_t diag1_used[2 * MAX_COORD + 1];
static uint8_t diag2_used[2 * MAX_COORD + 1];
static int initialized = 0;

/* Cache of results: a(1)...a(current_n).  The cache is grown
 * as needed.  res_len holds the number of stored terms. */
static seq_t *results = NULL;
static size_t res_cap = 0;
static size_t res_len = 0;

/* Current scanning position in antidiagonal order:  cur_s = i+j,
 * cur_i = i.  cur_i runs from 0 to cur_s, and cur_s increments
 * after a full diagonal is processed. */
static idx_t cur_s = 0;
static idx_t cur_i = 0;

/* Extend the cache so that it holds at least n terms.
 * The function scans the board square by square (in antidiagonal
 * order) until it has recorded the positions of n queens. */
static void extend_results(idx_t n)
{
    if (!initialized) {
        /* First time: clear all flag arrays. */
        memset(row_used, 0, sizeof(row_used));
        memset(col_used, 0, sizeof(col_used));
        memset(diag1_used, 0, sizeof(diag1_used));
        memset(diag2_used, 0, sizeof(diag2_used));
        initialized = 1;
    }

    while (res_len < n) {
        /* Current coordinate (i,j).  We assume cur_i <= cur_s. */
        idx_t i = cur_i;
        idx_t j = cur_s - cur_i;

        /* Bounds test: the static arrays are sized by MAX_COORD,
         * but for the required n this never triggers.  The test
         * keeps the function robust if the limit is exceeded. */
        if (i > (idx_t)MAX_COORD || j > (idx_t)MAX_COORD)
            return;   /* should never happen for the test cases */

        int d1 = (int)i - (int)j + MAX_COORD;  /* i-j range [-MAX, MAX] */
        int d2 = (int)i + (int)j;              /* i+j range [0, 2*MAX] */

        if (!row_used[i] && !col_used[j] && !diag1_used[d1] && !diag2_used[d2]) {
            /* Place a queen here. */
            row_used[i]   = 1;
            col_used[j]   = 1;
            diag1_used[d1] = 1;
            diag2_used[d2] = 1;

            /* Compute the global index of this square:
             * index = (s*(s+1))/2 + i, where s = i+j = cur_s. */
            seq_t s_seq = (seq_t)cur_s;
            seq_t index  = s_seq * (s_seq + 1) / 2 + (seq_t)i;

            /* Grow the result array if necessary. */
            if (res_len >= res_cap) {
                size_t newcap = res_cap ? res_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(results, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();  /* out of memory */
                results = tmp;
                res_cap = newcap;
            }
            results[res_len++] = index;
        }

        /* Advance to the next square in the current diagonal. */
        if (cur_i < cur_s) {
            ++cur_i;
        } else {        /* finished this diagonal */
            ++cur_s;
            cur_i = 0;
        }
    }
}

/* Public entry point required by the runtime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below offset; harmless */
    extend_results(n);
    return results[n - 1];
}

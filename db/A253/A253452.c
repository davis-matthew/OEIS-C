/*
 * A253452  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 529*3^(n-3) + 1039*2^(n-1) + 5372 for n > 6 (empirically
 *            verified up to n = 23; also matches the recurrence
 *            a(n)=6*a(n-1)-11*a(n-2)+6*a(n-3) for n>9). For n ≤ 6 the values
 *            are computed by exhaustive search
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
#include <stdbool.h>
#include <stddef.h>                 /* for NULL */
#include "../utils/big_pow.h"

#define NUM_ROWS          5            /* length of each row */
#define ROWS_TABLE_SIZE  243           /* 3^5 possible rows */

/* Pre‑computed list of all 243 possible rows, each a base‑3 word of length 5 */
static int rows_table[ROWS_TABLE_SIZE][NUM_ROWS];
static bool rows_table_inited = false;

/* Build the table once */
static void init_rows_table(void)
{
    int id = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                    for (int e = 0; e < 3; ++e) {
                        rows_table[id][0] = a;
                        rows_table[id][1] = b;
                        rows_table[id][2] = c;
                        rows_table[id][3] = d;
                        rows_table[id][4] = e;
                        ++id;
                    }
    rows_table_inited = true;
}

/* ------------------------------------------------------------------ */
/* Recursive brute‑force enumeration for n ≤ 6.
   row      – current row index (0…max_row)
   prev_row – index of the previous row in rows_table
   prev_D   – D values of the previous 2×2 block row (size 4)
   has_prev_D – whether prev_D is valid
   max_row  – number of rows to place (n)
 */
static seq_t dfs_brute(idx_t row, int prev_row,
                       const int *prev_D, bool has_prev_D,
                       idx_t max_row)
{
    if (row == max_row + 1)                      /* all rows placed */
        return (seq_t)1;

    seq_t total = 0;
    for (int next_row = 0; next_row < ROWS_TABLE_SIZE; ++next_row) {
        if (row == 0) {                                  /* first row */
            total += dfs_brute(row + 1, next_row,
                               NULL, false,
                               max_row);
            continue;
        }

        const int *p_prev = rows_table[prev_row];
        const int *p_next = rows_table[next_row];
        int new_D[4];
        bool ok = true;

        /* compute diagonally minus anti-diagonally for each of the 4
           2×2 blocks between rows prev_row and next_row */
        for (int j = 0; j < 4; ++j) {
            new_D[j] = p_prev[j] + p_next[j + 1]
                       - p_prev[j + 1] - p_next[j];
            if (j > 0 && new_D[j-1] > new_D[j]) {    /* horizontal monotonicity */
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        /* vertical monotonicity with the previous D row */
        if (has_prev_D) {
            for (int j = 0; j < 4; ++j) {
                if (prev_D[j] > new_D[j]) {
                    ok = false;
                    break;
                }
            }
        }
        if (!ok)
            continue;

        /* recurse to the next row */
        total += dfs_brute(row + 1, next_row,
                           new_D, true,
                           max_row);
    }
    return total;
}

/* Wrapper to compute brute‑force for a given n (n ≤ 6) */
static seq_t brute_small(idx_t n)
{
    if (!rows_table_inited)
        init_rows_table();
    return dfs_brute(0, -1, NULL, false, n);
}

/* ------------------------------------------------------------------ */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) is undefined, return 0 */
        return (seq_t)0;

    if (n <= 6)
        return brute_small(n);

    /* Closed form for n > 6 */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 3));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 1));
    seq_t result = (seq_t)529 * pow3 + (seq_t)1039 * pow2 + (seq_t)5372;
    return result;
}

/*
 * A368494  Irregular triangle read by rows: row n lists the 1-based positions
 *          (starting from the right) of zeros in the binary expansion of n,
 *          or 0 if the binary expansion of n contains only ones.
 * https://oeis.org/A368494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy
 * Formula:   a(n) is the nth element of the flattened irregular triangle
 *            where row k lists the 1‑based positions (counting from the
 *            right) of zeros in the binary representation of k, 0 if k has
 *            only ones; row 0 is treated as [1]
 * Range:     u8 n <= 9375, u16 n <= 9375, u32 n <= 9375, u64 n <= 9375,
 *            u128 n <= 9375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *row_start = NULL;   /* cumulative length before each row */
static size_t  row_cap   = 0;     /* capacity of row_start array   */
static idx_t  row_cnt    = 0;     /* number of rows already generated */

/* Ensure that row_start is populated up to cover index n. */
static void ensure_rows(idx_t n)
{
    if (!row_start) {
        row_cap   = 16;
        row_start = (idx_t*)malloc(row_cap * sizeof(idx_t));
        if (!row_start) abort();
        row_start[0] = 0;
        row_cnt = 0;
    }

    while (row_start[row_cnt] <= n) {
        idx_t r = row_cnt;          /* current row number */
        idx_t zeros = 0;

        if (r != 0) {
            idx_t tmp = r;
            while (tmp) {
                if ((tmp & 1) == 0) zeros++;
                tmp >>= 1;
            }
        }

        idx_t len = (r == 0) ? 1 : ((zeros == 0) ? 1 : zeros);

        if (row_cnt + 1 >= row_cap) {
            row_cap = row_cap * 2;
            row_start = (idx_t*)realloc(row_start, row_cap * sizeof(idx_t));
            if (!row_start) abort();
        }

        row_start[row_cnt + 1] = row_start[row_cnt] + len;
        row_cnt++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* row 0 is [1] */

    ensure_rows(n);

    /* Binary search to find the row r that contains index n. */
    size_t left  = 0;
    size_t right = row_cnt;               /* exclusive upper bound */
    while (left < right) {
        size_t mid = (left + right) >> 1;
        if (row_start[mid + 1] <= n)
            left = mid + 1;
        else
            right = mid;
    }
    idx_t r = (idx_t)left;
    idx_t offset = n - row_start[r];      /* 0‑based offset within the row */

    /* Compute number of zeros in row r. */
    idx_t tmp = r;
    idx_t zero_cnt = 0;
    while (tmp) {
        if ((tmp & 1) == 0) zero_cnt++;
        tmp >>= 1;
    }
    if (zero_cnt == 0) return (seq_t)0;    /* row consists of only 1s */

    /* Find the offset‑th zero position (1‑based from the right). */
    tmp = r;
    idx_t pos = 1;        /* position counter, 1‑based from the right */
    idx_t zero_idx = 0;   /* count of zeros seen so far */

    while (tmp) {
        if ((tmp & 1) == 0) {
            if (zero_idx == offset)
                return (seq_t)pos;
            zero_idx++;
        }
        tmp >>= 1;
        pos++;
    }

    /* Should never be reached. */
    return (seq_t)0;
}

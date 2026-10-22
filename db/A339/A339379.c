/*
 * A339379  Irregular triangle read by rows; the first row simply contains the
 *          value 1; given the succession of digits of the n-th row, say [d_0,
 *          ..., d_k], the (n+1)-th row is the succession of digits of [d_0,
 *          d_0+d_1, d_1+d_2, ..., d_{k-1}+d_k, d_k].
 * https://oeis.org/A339379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, easy
 * Formula:   a(n) is the concatenation of rows of an irregular triangle. Row
 *            0: [1]. If a row has digits [d0,…,dk], the next row is the
 *            digits of [d0, d0+d1, d1+d2, …, d_{k-1}+d_k, dk]. The sequence
 *            a(n) gives the n‑th such digit in this flattened triangle
 * Range:     u8 n <= 9660, u16 n <= 9660, u32 n <= 9660, u64 n <= 9660,
 *            u128 n <= 9660, big any n
 * Helpers:   append_sum_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_sum_digits.h"

/* Representation of a row: array of decimal digits (0‑9). */
typedef struct {
    uint8_t *data;  /* pointer to digit array */
    size_t   len;   /* number of digits */
} row_t;

/* Dynamic arrays holding all rows that have been generated. */
static row_t *rows     = NULL; /* array of rows */
static size_t rows_len = 0;    /* number of rows stored */
static size_t rows_cap = 0;    /* capacity of the array */

/* Ensure the rows array has at least 'need' capacity. */
static void ensure_rows_cap(size_t need)
{
    if (need <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 1;
    while (new_cap < need) new_cap *= 2;
    row_t *tmp = (row_t *)realloc(rows, new_cap * sizeof(row_t));
    rows = tmp;
    rows_cap = new_cap;
}

/* Compute the next row given the current row. The result is written into 'next'. */
static void compute_next_row(const row_t *cur, row_t *next)
{
    /* A new row has at most 2*(cur->len+1) digits. */
    size_t est = 2 * (cur->len + 1);
    uint8_t *digits = (uint8_t *)malloc(est * sizeof(uint8_t));
    size_t dlen = 0, dcap = est;

    /* First digit of the list is d0. */
    append_sum_digits(cur->data[0], &digits, &dlen, &dcap);

    /* Middle sums. */
    for (size_t i = 0; i + 1 < cur->len; ++i) {
        uint8_t sum = cur->data[i] + cur->data[i + 1];
        append_sum_digits(sum, &digits, &dlen, &dcap);
    }

    /* Last digit of the list is dk. */
    append_sum_digits(cur->data[cur->len - 1], &digits, &dlen, &dcap);

    /* Finalise new row. */
    next->data = digits;
    next->len  = dlen;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Convert idx to unsigned 64‑bit for internal use. */
    uint64_t idx = (uint64_t)n;

    /* Initialise the first row on the first call. */
    if (rows_len == 0) {
        row_t r0;
        r0.data = (uint8_t *)malloc(1 * sizeof(uint8_t));
        r0.data[0] = 1;
        r0.len = 1;
        ensure_rows_cap(1);
        rows[0] = r0;
        rows_len = 1;
    }

    /* Scan existing rows to find the one containing the desired index. */
    uint64_t cum = 0;          /* cumulative length of all processed rows */
    size_t row_idx = 0;        /* index of the row that will contain idx */
    size_t offset = 0;         /* position inside that row */

    for (size_t i = 0; i < rows_len; ++i) {
        if (cum + rows[i].len > idx) {
            row_idx = i;
            offset  = (size_t)(idx - cum);
            goto return_value;
        }
        cum += rows[i].len;
    }

    /* If we reached here, we need more rows to cover the requested index. */
    while (cum <= idx) {
        /* Compute next row from the last one. */
        row_t next;
        compute_next_row(&rows[rows_len - 1], &next);

        /* Append it to the stored rows. */
        ensure_rows_cap(rows_len + 1);
        rows[rows_len++] = next;

        /* Update cumulative length. */
        cum += next.len;

        /* Check if this new row covers the index. */
        if (cum > idx) {
            row_idx = rows_len - 1;
            offset  = (size_t)(idx - (cum - next.len));
            break;
        }
    }

return_value:
    /* Retrieve digit and return as seq_t. */
    uint8_t value = rows[row_idx].data[offset];
    return (seq_t)value;
}

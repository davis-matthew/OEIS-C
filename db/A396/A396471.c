/*
 * A396471  Irregular triangle read by rows: row n lists the coefficients of
 *          Product_{k=0..n-1} (1 + x^Fibonacci(k+2)).
 * https://oeis.org/A396471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   a(n) is the coefficient of x^k in ∏_{i=0}^{r-1} (1 +
 *            x^{Fibonacci(i+2)}), where r and k are determined by the
 *            cumulative lengths of the rows of the triangle: let L_r =
 *            Σ_{i=0}^{r} (Fibonacci(i+3) – 1), then row r contains the terms
 *            a(L_{r-1}), …, a(L_r – 1)
 * Range:     u8 n <= 2343, u16 n <= 9398, u32 n <= 9398, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   alloc_seq_array, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/alloc_seq_array.h"

#define MAX_ROWS 1000   /* plenty for n ≤ 10 000 */

/* Cached rows of the triangle. */
static seq_t *row_poly[MAX_ROWS];
static size_t row_len[MAX_ROWS];
static idx_t   cum_len[MAX_ROWS];
static idx_t   row_cnt = 0;

/* Build the next row in the triangle. */
static void build_next_row(void)
{
    if (row_cnt == 0) {
        /* Row 0: product of zero factors, polynomial 1. */
        row_poly[0] = alloc_seq_array(1);
        row_poly[0][0] = 1;
        row_len[0] = 1;
        cum_len[0] = 1;
        row_cnt = 1;
        return;
    }

    /* Previous row index is row_cnt – 1. */
    const idx_t prev_idx = row_cnt - 1;
    seq_t *prev = row_poly[prev_idx];
    const size_t prev_len = row_len[prev_idx];

    /* Exponent of the new factor (1 + x^F{row_cnt+1}). */
    const idx_t exp = (idx_t)fib(row_cnt + 1);

    const size_t new_len = prev_len + (size_t)exp;
    seq_t *new_poly = alloc_seq_array(new_len);

    /* Zero‑initialize the new polynomial. */
    for (size_t i = 0; i < new_len; ++i)
        new_poly[i] = 0;

    /* Convolution with (1 + x^exp). */
    for (size_t i = 0; i < prev_len; ++i) {
        new_poly[i] += prev[i];
        new_poly[i + (size_t)exp] += prev[i];
    }

    row_poly[row_cnt] = new_poly;
    row_len[row_cnt] = new_len;
    cum_len[row_cnt] = cum_len[prev_idx] + new_len;
    ++row_cnt;
}

/* Return the n-th member of OEIS sequence A396471. */
seq_t a(idx_t n)
{
    /* Ensure enough rows exist to cover index n. */
    while (row_cnt == 0 || n >= cum_len[row_cnt - 1]) {
        build_next_row();
    }

    /* Locate the row containing index n. */
    idx_t r = 0;
    while (r < row_cnt && n >= cum_len[r]) {
        ++r;
    }

    /* Offset within the found row. */
    const idx_t prev_cum = (r == 0) ? 0 : cum_len[r - 1];
    const idx_t col = n - prev_cum;

    return row_poly[r][(size_t)col];
}

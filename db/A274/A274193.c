/*
 * A274193  Triangular array read by rows: g(n,k) = 1 for n >= 0; g(n,k) = 0
 *          if k > n; g(n,k) = g(n-1,k-1) + g(n-1,3k) for n > 0, k > 1.
 * https://oeis.org/A274193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 1 (k == 0) T(n,k) = T(n-1,k-1) + (3*k <= n-1 ?
 *            T(n-1,3*k) : 0) (k >= 1) The sequence a(n) is obtained by
 *            reading the array T row by row (row 0, then row 1, etc.).
 *            Initial value: T(0,0) = 1
 * Range:     u8 n <= 352, u16 n <= 1129, u32 n <= 4096, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   row_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/row_leq.h"

/* ------------------------------------------------------------------ */
/*  Helper: binary search for the row that contains index n           */

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;

    /* Grow high until it strictly surpasses n. */
    while (row_leq(high, n))
        high <<= 1;

    /* Binary search in [low, high). */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (row_leq(mid, n))
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;   /* the desired row */
}

/* ------------------------------------------------------------------ */
/*  Storage for generated rows                                          */
/* ------------------------------------------------------------------ */
static seq_t **rows = NULL;          /* array of pointers to rows    */
static size_t rows_cap = 0;          /* current capacity of rows array */
static size_t max_row = 0;           /* largest row that has been computed */
static bool initialized = false;     /* have we created row 0 yet?  */

/* Ensure the rows array has room for at least 'cap' entries. */
static void ensure_rows_capacity(size_t cap)
{
    if (cap <= rows_cap)
        return;
    seq_t **new_rows = (seq_t **)realloc(rows, cap * sizeof(seq_t *));
    if (!new_rows)
        abort();
    rows = new_rows;
    rows_cap = cap;
}

/* Allocate and initialise row 0 (the first row). */
static void init(void)
{
    if (initialized)
        return;

    rows_cap = 1;
    rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
    if (!rows)
        abort();

    seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
    if (!row0)
        abort();
    row0[0] = (seq_t)1;     /* T(0,0) = 1 */
    rows[0] = row0;

    max_row = 0;
    initialized = true;
}

/* Compute all rows up to 'target' (inclusive), storing them in rows[]. */
static void compute_rows_upto(size_t target)
{
    if (target <= max_row)
        return;

    for (size_t i = max_row + 1; i <= target; ++i) {
        ensure_rows_capacity(i + 1);
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row)
            abort();

        row[0] = (seq_t)1;   /* T(i,0) = 1 */

        for (size_t k = 1; k <= i; ++k) {
            seq_t left = rows[i - 1][k - 1];
            seq_t right = 0;
            if (3 * k <= i - 1)          /* 3*k must be <= i-1 */
                right = rows[i - 1][3 * k];
            row[k] = left + right;
        }

        rows[i] = row;
    }
    max_row = target;
}

/* ------------------------------------------------------------------ */
/*  Public function: a(n)                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();

    idx_t r = find_row(n);                  /* row that contains n    */
    idx_t c = n - ((idx_t)r * (r + 1)) / 2; /* column within that row */

    compute_rows_upto((size_t)r);           /* make sure row r exists */
    return rows[(size_t)r][(size_t)c];
}

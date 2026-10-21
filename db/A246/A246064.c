/*
 * A246064  A variation of the Farey sequence read by rows; the row of order n
 *          is defined by the greatest whole numbers less than or equal to the
 *          values generated when the numerators of the Farey sequence (order
 *          n) are replaced by n itself.
 * https://oeis.org/A246064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A246064
   Formula:  a(n) = floor(row / denom)
   where `row` is the order of the Farey row that contains the
   n-th term, and `denom` is the denominator of the fraction that
   corresponds to that term in that row.
   The sequence is generated on demand by constructing, for each
   order m, the Farey sequence of order m and recording its
   denominators in the order 0/1, 1/m, … , 1/1.  The flattened
   list of denominators is then used to answer queries. */

#include <stdlib.h>

static seq_t *denom_cache = NULL;        /* flattened denominators */
static size_t  cache_len   = 0;           /* number of stored terms    */
static size_t  cache_cap   = 0;           /* capacity of denom_cache   */

static size_t *row_cum    = NULL;        /* cumulative terms after each row */
static size_t  rows_done  = 0;           /* number of rows generated   */
static size_t  row_cap    = 0;           /* capacity of row_cum       */

static idx_t current_row = 0;            /* last row generated         */

/* Ensure cache has room for at least `need` elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 512;
    if (new_cap < need) new_cap = need;
    denom_cache = (seq_t*)realloc(denom_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Ensure row_cum array has room for at least `need` entries */
static void ensure_row_cap(size_t need)
{
    if (row_cap >= need) return;
    size_t new_cap = row_cap ? row_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    row_cum = (size_t*)realloc(row_cum, new_cap * sizeof(size_t));
    row_cap = new_cap;
}

/* Generate the Farey denominators of order `m` and append them to cache.
   After the call, `rows_done` is increased by 1 and
   `row_cum[rows_done-1]` contains the total number of terms
   up to this row. */
static void generate_row(idx_t m)
{
    size_t start_len = cache_len;

    /* Start with 0/1 */
    ensure_cache_cap(cache_len + 1);
    denom_cache[cache_len++] = (seq_t)1;

    /* Classic Farey generation */
    seq_t a = (seq_t)0, b = (seq_t)1;
    seq_t c = (seq_t)1, d = (seq_t)m;

    while (!(c == (seq_t)1 && d == (seq_t)1)) {
        seq_t k = ((seq_t)m + b) / d;
        seq_t e = k * c - a;
        seq_t f = k * d - b;
        a = c; b = d;
        c = e; d = f;

        ensure_cache_cap(cache_len + 1);
        denom_cache[cache_len++] = b;
    }

    /* Append denominator of the final 1/1 */
    ensure_cache_cap(cache_len + 1);
    denom_cache[cache_len++] = (seq_t)1;

    /* Record end position for this row */
    rows_done++;
    ensure_row_cap(rows_done);
    row_cum[rows_done - 1] = cache_len;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but safe */

    /* Generate rows until we have enough terms */
    while (cache_len < (size_t)n)
        generate_row(++current_row);

    /* Find the row that contains term `n` */
    size_t row = 0;                      /* 0‑based index in row_cum */
    for (; row < rows_done; ++row)
        if (row_cum[row] >= (size_t)n)
            break;

    /* Previous cumulative count */
    size_t prev_cum = (row == 0) ? 0 : row_cum[row - 1];
    /* Position within the found row (0‑based) */
    size_t pos = (size_t)n - prev_cum - 1;

    seq_t denom  = denom_cache[prev_cum + pos];
    seq_t order  = (seq_t)(row + 1);          /* rows are 1‑based */

    return order / denom;                     /* integer division */
}

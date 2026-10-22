/*
 * A368281  Table T(n,k) n>=1, k>=1, read by downwards antidiagonals where the
 *          n-th row is the fixed point of the morphism 0->R(n), 1->R(n)0,
 *          where R(n) is the n-th repunit (A002275).
 * https://oeis.org/A368281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the flat table of A368281.
   It computes a(n) by first determining the row r and column c that
   correspond to the n‑th element of the table read downwards
   anti‑diagonally, then producing the fixed point of the morphism
   0 → "n 1's", 1 → "n 1's followed by 0" for that row.
   The row is generated lazily and stored to avoid repeated work. */
#include <stdlib.h>
#include <stddef.h>

static seq_t **rows_seq = NULL;   /* rows_seq[r] holds the sequence for row r (1‑based) */
static size_t *rows_len = NULL;   /* number of elements stored for each row */
static size_t rows_capacity = 0;  /* allocated capacity */

/* Ensure that the data arrays are large enough for row r. */
static void ensure_row_exists(idx_t r)
{
    idx_t req = r + 1;             /* rows array is 0‑based; skip index 0 */
    if (req > (idx_t)rows_capacity) {
        size_t new_cap = (size_t)req;
        if (new_cap < 2) new_cap = 2;
        rows_seq = (seq_t **)realloc(rows_seq, new_cap * sizeof(seq_t *));
        rows_len = (size_t *)realloc(rows_len, new_cap * sizeof(size_t));
        for (size_t i = rows_capacity; i < new_cap; ++i) {
            rows_seq[i] = NULL;
            rows_len[i] = 0;
        }
        rows_capacity = new_cap;
    }
}

/* Expand the fixed point for row r until it has at least needed_len elements. */
static void generate_row(idx_t r, idx_t needed_len)
{
    ensure_row_exists(r);

    if (rows_seq[r] == NULL) {
        /* initial word is just a single '1' */
        rows_seq[r] = (seq_t *)malloc(sizeof(seq_t));
        (rows_seq[r])[0] = 1;
        rows_len[r] = 1;
    }

    while (rows_len[r] < (size_t)needed_len) {
        size_t cur_len = rows_len[r];
        size_t count1 = 0;
        seq_t *cur = rows_seq[r];
        for (size_t i = 0; i < cur_len; ++i)
            if (cur[i] == 1) ++count1;

        size_t new_len = cur_len * (size_t)r + count1; /* each element gives r ones, plus a zero for each 1 */
        seq_t *new_seq = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            /* add r ones */
            for (size_t k = 0; k < (size_t)r; ++k)
                new_seq[pos++] = 1;
            /* add a zero if the original symbol was 1 */
            if (cur[i] == 1)
                new_seq[pos++] = 0;
        }
        free(rows_seq[r]);
        rows_seq[r] = new_seq;
        rows_len[r] = new_len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal s such that (s-1)*s/2 >= n.  Use 128‑bit to avoid overflow. */
    idx_t s = 2;
    while (1) {
        unsigned __int128 tri = (unsigned __int128)(s - 1) * s / 2;
        if (tri >= n) break;
        ++s;
    }

    idx_t prev_tri = (s - 2) * (s - 1) / 2;            /* terms before this diagonal */
    idx_t offset   = n - prev_tri;                    /* 1‑based index inside diagonal */
    idx_t r = offset;                                 /* row number (1‑based)   */
    idx_t c = s - offset;                             /* column number (1‑based) */

    /* Generate the needed element from row r. */
    generate_row(r, c);
    return rows_seq[r][c - 1];
}

/*
 * A326820  Irregular triangle read by rows; for n >= 0, the n-th row
 *          corresponds to the elements of the set {(n-k) OR k, k = 0..n}, in
 *          ascending order (where OR denotes the bitwise OR operator).
 * https://oeis.org/A326820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   not stated
 * Range:     u8 n <= 3287, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A326820 – Irregular triangle read by rows (bitwise OR values)
    Implements: the integer A326820, defined as the concatenation of rows,
    where the r‑th row (0‑based) contains the distinct values { (r‑k) | k : 0≤k≤r }
    in ascending order. */

/*  The code caches rows as they are computed.  For each row, only a bitset
    indicating present values and the row length are stored; the actual
    integer values are recovered on demand.  All memory operations use
    explicit casts so that the code compiles under C++ and C. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Row information structure */
struct RowInfo {
    unsigned char *present;   /* bitset: present[v] == 1 if v occurs in the row */
    size_t len;               /* number of distinct values in the row */
};

/* Global cache of rows */
static struct RowInfo *rows      = NULL;   /* array of computed rows          */
static size_t         rows_cap   = 0;      /* allocated capacity of rows       */
static size_t         rows_cnt   = 0;      /* number of rows currently stored  */

/* Cumulative lengths: cum[i] = total number of elements up to and including row i */
static size_t *cum          = NULL;       /* array of cumulative sums          */
static size_t  cum_cap       = 0;         /* allocated capacity for cum        */
static size_t  cum_cnt       = 0;         /* number of cumulative entries      */

/* Ensure the cache can hold at least 'need' entries, allocating more if needed. */
static void grow_cache(size_t need)
{
    if (need < rows_cap) return;               /* already enough space */

    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while (new_cap <= need) new_cap <<= 1;

    /* Reallocate rows array */
    struct RowInfo *new_rows = (struct RowInfo *)realloc(rows,
                                                        new_cap * sizeof(struct RowInfo));
    if (!new_rows) abort();
    rows = new_rows;
    rows_cap = new_cap;

    /* Reallocate cumulative sum array */
    size_t *new_cum = (size_t *)realloc(cum,
                                        new_cap * sizeof(size_t));
    if (!new_cum) abort();
    cum = new_cum;
    cum_cap = new_cap;
}

/* Compute the r‑th row (0‑based) and store its information in the cache. */
static void compute_row(idx_t r)
{
    size_t sz = (size_t)r + 1;                       /* values are in 0..r  */
    unsigned char *present = (unsigned char *)calloc(sz, 1);
    if (!present) abort();

    /* Mark all (r‑k) | k for k = 0..r */
    for (idx_t k = 0; k <= r; ++k) {
        seq_t val = ((seq_t)(r - k)) | ((seq_t)k);
        present[(size_t)val] = 1;
    }

    /* Count number of distinct values */
    size_t len = 0;
    for (size_t v = 0; v < sz; ++v) if (present[v]) ++len;

    /* Store information */
    rows[rows_cnt].present = present;
    rows[rows_cnt].len     = len;

    /* Update cumulative sum */
    cum[rows_cnt] = (rows_cnt == 0 ? 0 : cum[rows_cnt - 1]) + len;

    ++rows_cnt;
}

/* Ensure rows are computed such that index n is within the accumulated rows. */
static void ensure_rows_for_index(idx_t n)
{
    while (rows_cnt == 0 || (size_t)n >= cum[rows_cnt - 1]) {
        grow_cache(rows_cnt);
        compute_row(rows_cnt);        /* next row has number rows_cnt */
    }
}

/* Binary search for the row that contains the given index n. */
static size_t find_row_for_index(idx_t n)
{
    size_t lo = 0;
    size_t hi = rows_cnt - 1;
    size_t r  = rows_cnt - 1;

    while (lo <= hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((size_t)n < cum[mid]) {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return r;
}

/*  Return the n-th term of OEIS A326820.  */
seq_t a(idx_t n)
{
    ensure_rows_for_index(n);

    size_t r = find_row_for_index(n);
    size_t prev_cum = (r == 0 ? 0 : cum[r - 1]);
    size_t pos_in_row = (size_t)n - prev_cum;      /* 0‑based position within row */

    /* Find the pos_in_row‑th present value in ascending order. */
    size_t count = 0;
    size_t v = 0;
    size_t sz = (size_t)r + 1;
    unsigned char *present = rows[r].present;

    while (v < sz) {
        if (present[v]) {
            if (count == pos_in_row) return (seq_t)v;
            ++count;
        }
        ++v;
    }

    /* Should never reach here */
    abort();
}

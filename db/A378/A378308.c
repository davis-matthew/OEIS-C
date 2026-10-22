/*
 * A378308  Array read by antidiagonals: For k >= 3, row k-2 consists of the
 *          numbers with no digits 0 in bases 3 to k.
 * https://oeis.org/A378308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) is the nth entry of the array read by antidiagonals, *
 *            where row k–2 contains 1, 2 and every integer that has no zero *
 *            digit in any base d = 3,…,k. The function computes this value *
 *            on demand and memoises each row
 * Range:     u8 n <= 124, u16 n <= 308, u32 n <= 378, u64 n <= 378,
 *            u128 n <= 378, big any n
 * Helpers:   has_zero_digit_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_zero_digit_in_base.h"

typedef struct {
    size_t len;          /* number of stored values      */
    size_t cap;          /* allocated capacity           */
    seq_t  *data;        /* array of row values (1,2,…)  */
    seq_t  next_cand;    /* next natural number to test */
} RowCache;

/* Global row cache: row_cache[i] holds data for row i (1‑based). */
static RowCache *row_cache = NULL;
static size_t    cache_cap = 0;   /* allocated number of rows */

/* Ensure that row 'idx' (1‑based) exists and contains at least 'need'
 * elements.  Newly required elements are generated on demand.           */
static void ensure_row(idx_t idx, size_t need)
{
    /* Grow the array of RowCache structs if necessary.       */
    if ((size_t)idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)idx) new_cap *= 2;

        RowCache *tmp = (RowCache *)realloc(row_cache,
                                            new_cap * sizeof(RowCache));
        if (!tmp) exit(1);                   /* allocation failed */

        /* Initialise the newly created rows. */
        for (size_t k = cache_cap; k < new_cap; ++k) {
            tmp[k].len = 0;
            tmp[k].cap = 0;
            tmp[k].data = NULL;
            tmp[k].next_cand = (seq_t)3;
        }

        row_cache = tmp;
        cache_cap = new_cap;
    }

    RowCache *row = &row_cache[(size_t)idx];

    /* First use: allocate storage and insert the mandatory 1 and 2. */
    if (row->len == 0) {
        row->cap = 4;
        row->data = (seq_t *)malloc(row->cap * sizeof(seq_t));
        if (!row->data) exit(1);
        row->data[0] = (seq_t)1;
        row->data[1] = (seq_t)2;
        row->len  = 2;
        row->next_cand = (seq_t)3;
    }

    /* Generate additional elements until the required size is reached. */
    while (row->len < need) {
        seq_t cand = row->next_cand;
        bool zero = false;

        /* Test bases 3 through (idx+2) for a zero digit. */
        for (size_t base = 3; base <= (size_t)(idx + 2); ++base) {
            if (has_zero_digit_in_base(cand, (unsigned int)base)) {
                zero = true;
                break;
            }
        }

        if (!zero) {
            if (row->len == row->cap) {
                row->cap *= 2;
                seq_t *newp = (seq_t *)realloc(row->data,
                                               row->cap * sizeof(seq_t));
                if (!newp) exit(1);
                row->data = newp;
            }
            row->data[row->len++] = cand;
        }
        ++row->next_cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */

    /* Find the antidiagonal index 's' such that
     * (s−1)*s/2 ≥ n  and  (s−2)*(s−1)/2 < n.        */
    idx_t low  = 2;
    idx_t high = 2;
    while ((((unsigned __int128)(high - 1) * high) / 2ULL) < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = ((unsigned __int128)(mid - 1) * mid) / 2ULL;
        if (val >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t s = low;   /* antidiagonal index (rows+cols = s) */

    unsigned __int128 prev_sum = ((unsigned __int128)(s - 2) * (s - 1)) / 2ULL;
    idx_t   i = n - (idx_t)prev_sum;   /* row number (1‑based) */
    idx_t   j = s - i;                  /* column number (1‑based) */

    if (j <= 2) return (j == 1) ? (seq_t)1 : (seq_t)2;

    /* Ensure the requested element exists. */
    ensure_row(i, (size_t)j);
    return row_cache[(size_t)i].data[(size_t)(j - 1)];
}

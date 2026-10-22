/*
 * A360371  Triangle read by rows: lexicographically earliest sequence of
 *          distinct positive integers such that each column contains only
 *          multiples of the first number in that column. See example.
 * https://oeis.org/A360371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   ensure_col, ensure_used_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Generates the triangle row by row, selecting for each
   position the smallest unused positive integer that satisfies the
   column‑multiple condition.  The values are stored in a static cache
   so that calls with arbitrary n are efficient.  Memoisation is used
   to allow calls in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_col.h"
#include "../utils/ensure_used_value.h"

/*--- helpers to grow dynamic arrays --------------------------------*/

static void ensure_cache(idx_t *cap_p, seq_t **arr_p, idx_t idx)
{
    size_t cap = (size_t)*cap_p;
    size_t needed = (size_t)(idx + 1);
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(*arr_p, (new_cap + 1) * sizeof(seq_t));
    if (!new_arr) abort();
    *cap_p = new_cap;
    *arr_p = new_arr;
}

/*--- main sequence function -----------------------------------------*/

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache      = NULL;  /* 1‑based cache */
    static idx_t cache_cap  = 0;
    static idx_t max_comp   = 0;       /* largest index computed so far */

    static bool *used  = NULL;         /* used[v] == true if v has appeared */
    static idx_t used_cap = 0;
    static idx_t next_unused = 1;      /* next integer to try for a new column */

    static seq_t *col_base = NULL;     /* first element of each column */
    static idx_t *col_mult = NULL;     /* next multiplier for each column */
    static idx_t col_cap  = 0;

    /* persistent state for row bookkeeping */
    static idx_t cur_row = 0;          /* current row number (when generating) */
    static idx_t tri     = 0;          /* triangular number of cur_row */

    /* Ensure cache can hold up to n */
    ensure_cache(&cache_cap, &cache, n);

    if (n > max_comp) {
        /* generate all terms from max_comp+1 up to n */
        for (idx_t idx = max_comp + 1; idx <= n; ++idx) {
            /* advance to the correct row for this position */
            while (idx > tri) {
                ++cur_row;
                tri = cur_row * (cur_row + 1) / 2;
            }

            /* column index within the current row (1‑based) */
            idx_t col = idx - (tri - cur_row);

            /* make room for this column */
            ensure_col(&col_cap, &col_base, &col_mult, col);

            if (col_base[col] == 0) {          /* new column */
                /* find smallest unused integer */
                while (true) {
                    ensure_used_value(&used_cap, &used, next_unused);
                    if (!used[next_unused]) break;
                    ++next_unused;
                }
                seq_t val = (seq_t)next_unused;
                used[next_unused] = true;
                ++next_unused;

                cache[idx]    = val;
                col_base[col] = val;
                col_mult[col] = 2;             /* next multiplier starts at 2 */
            } else {                           /* existing column */
                seq_t base   = col_base[col];
                idx_t mult   = col_mult[col];
                seq_t val;
                while (true) {
                    val = base * (seq_t)mult;
                    size_t val_idx = (size_t)val;
                    ensure_used_value(&used_cap, &used, (idx_t)val_idx);
                    if (!used[val_idx]) {
                        used[val_idx] = true;
                        cache[idx]    = val;
                        col_mult[col] = mult + 1;
                        break;
                    }
                    ++mult;
                }
            }
        }
        max_comp = n;
    }

    return cache[n];
}

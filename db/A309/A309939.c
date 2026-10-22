/*
 * A309939  Triangle read by rows: T(n,k) is the number of compositions of n
 *          with k parts and differences all equal to 1, 0, or -1.
 * https://oeis.org/A309939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309939
 * Triangle read by rows: T(n,k) = number of compositions of n into k parts with
 * successive differences in {-1, 0, 1}.  This function returns the flattened
 * triangle read by rows with offset 1: a(1)=1, a(2)=1, a(3)=1, ...
 *
 * Implements a dynamic programming solution that builds the triangle up to the
 * required row whenever a larger index is requested.  The DP state is
 *   dp_len[sum][last]  – number of sequences of length len ending with value
 *   last and having total sum sum.  For each new len we compute the state
 *   from the previous len and accumulate the totals for all sums to obtain
 *   the triangle entries T[sum][len].
 *
 * The constructed triangle is then flattened into a contiguous array which
 * allows O(1) look‑up of any a(n).  The implementation uses only addition,
 * subtraction, and array indexing on the type seq_t, as required.  All
 * allocations are carried out with calloc/realloc from <stdlib.h>.
 */

#include <stdlib.h>

/* Static state for memoisation */
static seq_t *triangle_flat = NULL;      /* flattened triangle */
static size_t flat_len = 0;              /* number of entries in triangle_flat */
static idx_t  rows_computed = 0;         /* highest row already built */

static seq_t *dp_cur = NULL;             /* DP for current length */
static seq_t *dp_next = NULL;            /* DP for next length */
static seq_t *Tmat   = NULL;             /* matrix of T[sum][len] */
static size_t stride = 0;                /* stride = rows_computed + 1 */

/* Build the triangle up to the given number of rows */
static void build_table(idx_t max_row)
{
    size_t r = (size_t)max_row;        /* maximum row index (1‑based) */
    size_t new_stride = r + 1;         /* stride for the 2‑D arrays */

    /* Allocate / reallocate arrays */
    Tmat   = (seq_t *)realloc(Tmat,   new_stride * new_stride * sizeof(seq_t));
    dp_cur = (seq_t *)realloc(dp_cur, new_stride * new_stride * sizeof(seq_t));
    dp_next= (seq_t *)realloc(dp_next,new_stride * new_stride * sizeof(seq_t));

    /* Initialise to zero */
    if (Tmat)   memset(Tmat,   0, new_stride * new_stride * sizeof(seq_t));
    if (dp_cur) memset(dp_cur, 0, new_stride * new_stride * sizeof(seq_t));
    if (dp_next)memset(dp_next,0, new_stride * new_stride * sizeof(seq_t));

    stride = new_stride;

    /* Length = 1 : only one part, the whole sum */
    for (size_t val = 1; val <= r; ++val) {
        dp_cur[val * stride + val] = 1;      /* dp[sum=val][last=val] = 1 */
        Tmat[val * stride + 1]   = 1;         /* T[sum=val][len=1]   = 1 */
    }

    /* Length >= 2 : dynamic programming */
    for (size_t len = 2; len <= r; ++len) {
        /* compute dp_next for sequences of length = len */
        memset(dp_next, 0, stride * stride * sizeof(seq_t));

        for (size_t sum = 1; sum <= r; ++sum) {
            for (size_t last = 1; last <= sum; ++last) {
                size_t prev_sum = sum - last;
                if (prev_sum == 0) continue;     /* cannot reach zero with positive parts */

                seq_t total = 0;

                /* previous part may be last-1, last, or last+1 */
                if (last >= 2 && last-1 <= prev_sum)
                    total += dp_cur[prev_sum * stride + (last-1)];
                if (last <= prev_sum)
                    total += dp_cur[prev_sum * stride + last];
                if (last + 1 <= prev_sum)
                    total += dp_cur[prev_sum * stride + (last+1)];

                dp_next[sum * stride + last] = total;
            }
        }

        /* accumulate totals for all sums to fill T[sum][len] */
        for (size_t sum = len; sum <= r; ++sum) {
            seq_t row_total = 0;
            for (size_t last = 1; last <= sum; ++last)
                row_total += dp_next[sum * stride + last];
            Tmat[sum * stride + len] = row_total;
        }

        /* prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur     = dp_next;
        dp_next    = tmp;
    }

    /* Flatten the triangle into a one‑dimensional array */
    size_t new_flat_len = r * (r + 1) / 2;
    triangle_flat = (seq_t *)realloc(triangle_flat, new_flat_len * sizeof(seq_t));

    size_t pos = 0;
    for (size_t sum = 1; sum <= r; ++sum) {
        for (size_t len = 1; len <= sum; ++len)
            triangle_flat[pos++] = Tmat[sum * stride + len];
    }

    flat_len = new_flat_len;
    rows_computed = r;
}

/* Return the n‑th term of A309939 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for index 0, return 0 */

    /* Determine the row that contains index n */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) ++r;
    r++;                         /* rows are 1‑based */

    if (r > rows_computed)
        build_table(r);           /* build up to the required row */

    return triangle_flat[n-1];
}

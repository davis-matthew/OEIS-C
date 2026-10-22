/*
 * A355292  a(n) = Sum_{k=1..n} |Stirling1(n,k)| * Catalan(k-1).
 * https://oeis.org/A355292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} |Stirling1(n,k)| * Catalan(k-1) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Assuming the helper function catalan(idx_t k) is available. */
#include "../utils/catalan.h"

static seq_t *prev_row = NULL;       /* Stirling numbers S1(n-1, k)  */
static seq_t *cur_row  = NULL;       /* Stirling numbers S1(n,   k)  */
static seq_t *catalan_arr = NULL;    /* Catalan values C_{k-1}       */
static seq_t *result_arr = NULL;     /* a(n) values                 */

static size_t prev_cap = 0;
static size_t cur_cap  = 0;
static size_t catalan_cap = 0;
static size_t result_cap  = 0;

static idx_t max_computed = 0;       /* largest n for which we have a(n) */

/* Ensure a buffer has room for at least 'need' elements. */
static void ensure_cap(seq_t **buf, size_t *cap, size_t need)
{
    if (*cap < need) {
        *cap = need + 16;                 /* small slack */
        *buf = (seq_t *)realloc(*buf, *cap * sizeof(seq_t));
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n==0 is undefined but return 0 */

    /* Allocate or grow the Stirling row buffers. */
    ensure_cap(&prev_row, &prev_cap, n + 1);
    ensure_cap(&cur_row,  &cur_cap,  n + 1);

    /* Allocate or grow the Catalan buffer up to n-1. */
    ensure_cap(&catalan_arr, &catalan_cap, (idx_t)(n > 0 ? n : 1));
    /* Allocate or grow the result buffer. */
    ensure_cap(&result_arr, &result_cap, n + 1);

    /* Initialise the base row for n = 0: S1(0,0)=1, others 0. */
    if (max_computed == 0) {
        prev_row[0] = 1;
        for (idx_t i = 1; i <= n; ++i) prev_row[i] = 0;
        max_computed = 0;
    }

    /* Compute Catalan numbers up to n-1 if needed. */
    if (max_computed > 0) {
        for (idx_t k = 1; k <= n - 1; ++k)
            if (catalan_arr[k] == 0) catalan_arr[k] = catalan(k);
    } else {
        catalan_arr[0] = 1;  /* C_0 = 1 */
        for (idx_t k = 1; k <= n - 1; ++k)
            catalan_arr[k] = catalan(k);
    }

    /* If already computed up to n, return cached value. */
    if (max_computed >= n) return result_arr[n];

    /* Compute rows from max_computed+1 up to n. */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* Compute Stirling numbers for row m. */
        cur_row[0] = 0;  /* S1(m,0)=0 for m>0 */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term1 = prev_row[k - 1];
            seq_t term2 = prev_row[k];
            term2 = term2 * (seq_t)(m - 1);   /* (m-1)*S1(m-1,k) */
            cur_row[k] = term1 + term2;
        }

        /* Compute a(m) = sum_{k=1..m} S1(m,k) * C_{k-1}. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += cur_row[k] * catalan_arr[k - 1];

        result_arr[m] = sum;

        /* Prepare for next iteration: swap rows. */
        seq_t *tmp = prev_row;
        prev_row = cur_row;
        cur_row  = tmp;
    }

    max_computed = n;
    return result_arr[n];
}

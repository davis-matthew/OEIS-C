/*
 * A384496  a(n) = Sum_{k=0..n} binomial(n,k)^3 * abs(Stirling1(2*k,k)) *
 *          abs(Stirling1(2*n-2*k,n-k)).
 * https://oeis.org/A384496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * * abs(Stirling1(2*k,k)) *
 *            abs(Stirling1(2*n-2*k,n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following types are supplied by the build.  Do not define them. */
extern seq_t binomial(idx_t n, idx_t k);   /* positive binomial coefficient */

/* Memoised data for the central unsigned Stirling numbers of the first
 * kind:  f[m] = abs(Stirling1(2*m,m)).  f_arr[0] = 1 is always valid,
 * even for n = 0.  For n > 0 we build the tables on demand.
 */
static seq_t *f_arr = NULL;     /* f[0..max_n] */
static size_t f_cap = 0;        /* capacity of f_arr */

static seq_t *prev_row = NULL;  /* row for n-1 */
static seq_t *curr_row = NULL;  /* row for current n */
static size_t row_cap = 0;      /* capacity of row arrays (size of last computed n + 1) */
static idx_t current_max_n = 0; /* largest n for which the Stirling rows have been filled */

/* Ensure Stirling numbers up to 2*desired_n are available and f_arr[0..desired_n]
 * is filled.  The routine is called only when desired_n > max_pre. */
static void
ensure_sstirling(idx_t desired_n)
{
    idx_t needed_n2 = 2 * desired_n;             /* we need rows up to 2*desired_n */
    if (current_max_n >= needed_n2) return;      /* already done */

    /* Resize rows if needed.  They must be able to hold indices up to needed_n2. */
    if ((size_t)needed_n2 + 1 > row_cap) {
        size_t new_cap = (size_t)needed_n2 + 1;
        seq_t *new_prev = (seq_t *)realloc(prev_row, new_cap * sizeof(seq_t));
        seq_t *new_curr = (seq_t *)realloc(curr_row, new_cap * sizeof(seq_t));
        if (!new_prev || !new_curr) {
            /* Allocation failure – aborting compute will cause a runtime error. */
            abort();
        }
        prev_row = new_prev;
        curr_row = new_curr;
        row_cap   = new_cap;
    }

    /* Resize f_arr if needed.  It must hold indices up to desired_n. */
    if ((size_t)desired_n + 1 > f_cap) {
        size_t new_cap = (size_t)desired_n + 1;
        seq_t *new_f = (seq_t *)realloc(f_arr, new_cap * sizeof(seq_t));
        if (!new_f) abort();
        f_arr = new_f;
        f_cap = new_cap;
    }

    /* Initialise the first row (n = 0).  prev_row[0] = 1, f_arr[0] = 1. */
    if (current_max_n == 0) {
        prev_row[0] = 1;        /* c(0,0) */
        f_arr[0]    = 1;        /* c(0,0) */
    }

    /* Compute rows from current_max_n+1 up to needed_n2.  Every even row
     * contributes one central Stirling number.  The recurrence for
     * unsigned Stirling numbers of the first kind is:
     *
     *    S(n,k) = S(n-1,k-1) + (n-1) * S(n-1,k)       (for k>0)
     *    S(n,0) = 0   (for n>0),  S(n,n) = 1
     *
     * We keep the two latest rows in prev_row and curr_row. */
    for (idx_t n = current_max_n + 1; n <= needed_n2; ++n) {
        curr_row[0] = 0;
        for (idx_t k = 1; k < n; ++k) {
            seq_t t1 = prev_row[k - 1];
            seq_t t2 = prev_row[k] * (seq_t)(n - 1);
            curr_row[k] = t1 + t2;
        }
        curr_row[n] = 1;   /* S(n,n) = 1 */

        /* If n is even, store the central value. */
        if ((n & 1) == 0) {
            idx_t m = n >> 1;
            f_arr[m] = curr_row[m];
        }

        /* Swap rows for the next iteration. */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
    }

    current_max_n = needed_n2;
}

/*-----------------------------------------------------------*/
seq_t
a(idx_t n)
{
    /* Base case */
    if (n == 0) return 1;

    /* Build required Stirling tables and central values. */
    ensure_sstirling(n);

    seq_t sum   = 0;
    seq_t binom = 1;            /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(n,k)^3 */
        seq_t bm = binom * binom;
        bm = bm * binom;

        /* multiply by the two central Stirling numbers */
        seq_t term = bm * f_arr[k] * f_arr[n - k];
        sum += term;

        /* update binomial to binomial(n,k+1) */
        if (k < n) {
            seq_t num = binom * (seq_t)(n - k);
            binom = num / (seq_t)(k + 1);
        }
    }
    return sum;
}

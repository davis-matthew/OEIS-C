/*
 * A385752  a(n) = Sum_{k=0..n} Stirling1(n,k) * (n!/k!)^2.
 * https://oeis.org/A385752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k) * (n!/k!)^2
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *prev_row = NULL;   /* Stirling numbers for n-1 */
static seq_t *curr_row = NULL;   /* Stirling numbers for current n */
static size_t prev_size = 0;     /* length of prev_row (i.e. n-1+1) */

static seq_t *seq_vals = NULL;   /* a(n) values */
static idx_t   max_comp = 0;     /* highest n computed */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialisation for n = 0 */
    if (prev_row == NULL) {
        prev_row = (seq_t *)malloc(sizeof(seq_t));
        prev_row[0] = 1;        /* S1(0,0) = 1 */

        prev_size = 1;

        seq_vals = (seq_t *)malloc(sizeof(seq_t));
        seq_vals[0] = 1;        /* a(0) = 1 */

        max_comp = 0;
    }

    /* Return cached value if already computed */
    if (n <= max_comp)
        return seq_vals[n];

    /* Compute rows up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* Allocate buffer for current row (length i+1) */
        if (curr_row == NULL || prev_size < (size_t)(i + 1))
            curr_row = (seq_t *)realloc(curr_row, sizeof(seq_t) * (i + 1));

        /* Stirling numbers of the first kind: S1(i,k) = S1(i-1,k-1) - (i-1)*S1(i-1,k) */
        curr_row[0] = 0;                           /* S1(i,0) = 0 for i>0 */
        seq_t im1 = (seq_t)(i - 1);                /* (i-1) as seq_t */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t a_term = (k - 1 < prev_size) ? prev_row[k - 1] : 0;
            seq_t b_term = (k < prev_size) ? prev_row[k] * im1 : 0;
            curr_row[k] = a_term - b_term;
        }

        /* Compute a(i) = Σ_k S1(i,k) * (i!/k!)^2 by back‑to‑front accumulation */
        seq_t a_val = 0;
        seq_t prod = 1;   /* (i!/i!)^2 = 1 */
        for (idx_t k = i; ; --k) {
            a_val += curr_row[k] * prod;
            if (k == 0)
                break;
            /* prod = prod * k^2   for next smaller k        */
            prod *= (seq_t)k * (seq_t)k;
        }

        /* Store the value */
        seq_vals = (seq_t *)realloc(seq_vals, sizeof(seq_t) * (i + 1));
        seq_vals[i] = a_val;

        /* Prepare for next iteration: swap rows */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
        prev_size = i + 1;
    }

    max_comp = n;
    return seq_vals[n];
}

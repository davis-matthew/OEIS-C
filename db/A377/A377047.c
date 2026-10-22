/*
 * A377047  Antidiagonal-sums of the array A377046(n,k) = n-th term of k-th
 *          differences of nonsquarefree numbers (A013929).
 * https://oeis.org/A377047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 33, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A377047 – Antidiagonal sums of the triangle A377046.
   The value a(n) is the sum of the elements on the n‑th
   antidiagonal of the triangle whose k‑th row is the k‑th forward
   difference of the nonsquarefree numbers (A013929).
   The implementation builds the required nonsquarefree numbers
   on demand and iteratively computes forward differences.
*/
#include <stdlib.h>
#include <stdbool.h>

/*----- storage for nonsquarefree numbers -----*/
static seq_t *S = NULL;          /* array of nonsquarefree numbers   */
static idx_t S_count   = 0;      /* number stored                    */
static idx_t S_capacity = 0;     /* allocated capacity               */
static idx_t next_candidate = 0; /* next integer to test             */

/*----- ensure S has at least 'needed' elements -----*/
static void ensure_S(idx_t needed)
{
    /* enlarge the array if necessary */
    if (S_capacity < needed) {
        idx_t new_cap = S_capacity ? S_capacity << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = (seq_t *)realloc(S, sizeof(seq_t) * new_cap);
        if (!p) abort();
        S = p;
        S_capacity = new_cap;
    }

    /* generate more nonsquarefree numbers if required */
    while (S_count < needed) {
        ++next_candidate;
        /* test whether next_candidate is nonsquarefree */
        if (next_candidate >= 4) {
            bool nonsq = false;
            for (idx_t p = 2; p <= next_candidate / p; ++p) {
                idx_t sq = p * p;
                if (next_candidate % sq == 0) { nonsq = true; break; }
            }
            if (nonsq) {
                S[S_count] = (seq_t)next_candidate;
                ++S_count;
            }
        }
    }
}

/*----- temporary buffer for forward differences -----*/
static seq_t *row_buf = NULL;
static idx_t row_capacity = 0;

/*----- ensure the row buffer can hold 'need' elements -----*/
static void ensure_row_capacity(idx_t need)
{
    if (row_capacity < need) {
        row_capacity = need;
        seq_t *p = (seq_t *)realloc(row_buf, sizeof(seq_t) * row_capacity);
        if (!p) abort();
        row_buf = p;
    }
}

/*=================================================================*/
/*                      main sequence function                     */
/*=================================================================*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1 */

    /* make sure we have the first n nonsquarefree numbers */
    ensure_S(n);

    /* build a buffer for computing forward differences */
    ensure_row_capacity(n);

    /* copy the first n nonsquarefree numbers into the buffer */
    for (idx_t i = 0; i < n; ++i) row_buf[i] = S[i];

    seq_t sum = 0;
    idx_t cur_len = n;                  /* current length of the row */

    for (idx_t k = 0; k < n; ++k) {
        /* add the element on the present antidiagonal */
        sum += row_buf[cur_len - 1];

        if (k == n - 1) break;          /* last element reached */

        /* compute the next row of forward differences in place */
        for (idx_t i = 0; i < cur_len - 1; ++i)
            row_buf[i] = row_buf[i + 1] - row_buf[i];
        --cur_len;
    }

    return sum;
}

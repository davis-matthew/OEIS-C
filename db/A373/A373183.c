/*
 * A373183  Irregular triangle T(n, k), n >= 0, k > 0, read by rows with row
 *          polynomials R(n, x) such that R(2n+1, x) = x*R(n, x) for n >= 0,
 *          R(2n, x) = x*(R(n, x+1) - R(n, x)) for n > 0 with R(0, x) = x.
 * https://oeis.org/A373183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: R(2n+1, x) = x * R(n, x)          (n ≥ 0)
   Recurrence: R(2n,   x) = x * ( R(n, x+1) – R(n, x) )  (n > 0)
   Base: R(0, x) = x
   a(n) = first element of row 0, then row 1 elements, then row 2, …
   Row n has length popcount(n) + 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t a, idx_t b); */

static seq_t **rows = NULL;      /* array of pointers to coefficient arrays */
static size_t *row_len = NULL;   /* number of elements in each row        */
static idx_t *row_start = NULL;  /* flat index where each row starts       */
static size_t capacity = 0;      /* allocated capacity of the three arrays */
static size_t row_count = 0;     /* number of rows that have been built    */
static idx_t next_start = 0;      /* flat index of the first element of the
                                   row to be built next                                        */

/* resize static arrays when needed */
static void grow_arrays(void)
{
    size_t newcap = capacity == 0 ? 16 : capacity << 1;
    rows      = (seq_t **)realloc(rows,      newcap * sizeof(seq_t *));
    row_len   = (size_t *)realloc(row_len,   newcap * sizeof(size_t));
    row_start = (idx_t *)realloc(row_start, newcap * sizeof(idx_t));
    capacity  = newcap;
}

/* return a(n) */
seq_t a(idx_t n)
{
    /* Build rows until the requested index n is covered. */
    while (next_start <= n) {
        idx_t r = (idx_t)row_count;        /* new row index */
        if (row_count >= capacity)
            grow_arrays();

        /* Length of row r: popcount(r)+1. */
        size_t L = __builtin_popcountll((unsigned long long)r) + 1;
        row_len[r] = L;
        row_start[r] = next_start;

        /* Allocate space for the row coefficients. */
        seq_t *row = (seq_t *)malloc(L * sizeof(seq_t));
        rows[r] = row;

        /* Compute the row according to the defining recurrences. */
        if (r == 0) {                     /* R(0,x)=x  →  T(0,1)=1 */
            row[0] = (seq_t)1;
        } else if (r & 1) {               /* odd: R(2m+1,x)=x*R(m,x) */
            idx_t m = r >> 1;
            row[0] = 0;                   /* T(2m+1,1)=0 */
            /* shift coefficients of row m up by one position */
            for (size_t i = 0; i < row_len[m]; ++i)
                row[i+1] = rows[m][i];
        } else {                          /* even: R(2m,x)=x*(R(m,x+1)-R(m,x)) */
            idx_t m = r >> 1;
            size_t LM = row_len[m];
            seq_t *A = rows[m];           /* coefficients of row m */
            for (size_t t = 0; t < L; ++t) { /* t = exponent index of difference */
                seq_t val = 0;
                /* sum over k from max(1, t) to LM */
                size_t k_start = (t == 0 ? 1 : t);
                for (size_t k = k_start; k <= LM; ++k) {
                    seq_t comb = binomial((idx_t)k, (idx_t)t);
                    val += A[k-1] * comb;
                }
                if (t >= 1)                 /* subtract A_t if it exists */
                    val -= A[t-1];
                row[t] = val;
            }
        }

        /* Update indices for the next iteration. */
        next_start += L;
        ++row_count;
    }

    /* Locate the requested term within the last computed row. */
    idx_t r = (idx_t)(row_count - 1);
    size_t pos = (size_t)(n - row_start[r]);   /* zero‑based position in row */
    return rows[r][pos];
}

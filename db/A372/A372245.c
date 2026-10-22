/*
 * A372245  Triangular array T(n,k) read by rows: column k is the expansion of
 *          e.g.f: exp(-2*x)*(exp(x)-1)^k/(2-exp(x)).
 * https://oeis.org/A372245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n)-th element of the triangular array T(r,k)
 * read by rows, where T(r,k) is defined by the e.g.f.  
 *   G_k(x) = exp(-2x) * (exp(x)-1)^k / (2-exp(x))
 * The array satisfies:
 *     T(r,0) = g_r   with  g_0 = 1 and   g_r = (-2)^r + sum_{j<r} binom(r,j)*g_j
 *     T(r,k) = sum_{j<r} binom(r,j)*T(j,k-1)   for k>0
 * This implementation builds the triangle up to the row needed for any n.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

/* Integer square‑root (floor) of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32);          /* sqrt(2^64-1) < 2^32      */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Dynamic storage for the triangular array. */
static seq_t **T_rows = NULL;          /* array of row pointers      */
static idx_t rows_allocated = 0;       /* number of rows we have a slot for   */
static idx_t max_row_computed = 0;     /* last row that has been filled   */

/* Allocate space for rows up to req_row (inclusive) and compute them. */
static void ensure_rows(idx_t req_row)
{
    if (req_row < max_row_computed)
        return;                                    /* already ready */

    /* Extend storage for row pointers if needed. */
    if (req_row + 1 > rows_allocated) {
        idx_t new_cap = req_row + 1;
        T_rows = (seq_t **)realloc(T_rows, new_cap * sizeof(seq_t *));
        for (idx_t i = rows_allocated; i < new_cap; ++i)
            T_rows[i] = NULL;
        rows_allocated = new_cap;
    }

    /* Build rows from max_row_computed up to req_row. */
    for (idx_t r = max_row_computed; r <= req_row; ++r) {
        /* Allocate current row. */
        T_rows[r] = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));

        /* Compute the first column: T(r,0) = (-2)^r + sum_{j<r} binom(r,j)*T(j,0). */
        if (r == 0) {
            T_rows[0][0] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            seq_t pow_neg2 = big_pow((seq_t)-2, (seq_t)r);
            for (idx_t j = 0; j < r; ++j) {
                /* binom(r,j) in seq_t and T_rows[j][0] are seq_t as well. */
                sum += binom(r, j) * T_rows[j][0];
            }
            T_rows[r][0] = pow_neg2 + sum;
        }

        /* Compute remaining columns k = 1..r. */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < r; ++j) {
                if (k - 1 <= j)            /* T(j,k-1) exists only when k-1 <= j. */
                    sum += binom(r, j) * T_rows[j][k - 1];
            }
            T_rows[r][k] = sum;
        }
    }
    max_row_computed = req_row + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    unsigned __int128 tmp = (unsigned __int128)n * 8 + 1;
    idx_t r = (isqrt_u64((idx_t)tmp) - 1) / 2;

    idx_t row_start = r * (r + 1) / 2;
    idx_t col = n - row_start;

    ensure_rows(r);                 /* Build triangle up to required row. */
    return T_rows[r][col];
}

/*
 * A317503  Triangle read by rows: T(0,0) = 1; T(n,k) = -2 T(n-1,k) + 3 *
 *          T(n-3,k-1) for k = 0..floor(n/3); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   T(r,k) = (-2)^(r-3k) * 3^k * C(r-2k,k) (0 ≤ k ≤ ⌊r/3⌋) a(n) is
 *            the triangle read by rows, i.e. a(n)=T(r,k) where r is the row
 *            containing the n‑th element and k the column in that row
 * Range:     i8 n <= 12, i16 n <= 35, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

static idx_t *cum = NULL;          /* cumulative element counts per row   */
static size_t cum_len = 0;         /* number of rows stored in cum       */

/* Ensure cum array contains counts up to at least row r (inclusive). */
static void ensure_cum_row(idx_t r) {
    while (cum_len <= (size_t)r) {
        idx_t prev = (cum_len == 0) ? 0 : cum[cum_len - 1];
        idx_t row_len = (idx_t)(cum_len / 3 + 1);    /* elements in row cum_len */
        idx_t new_val = prev + row_len;
        idx_t *tmp = (idx_t *)realloc(cum, (cum_len + 1) * sizeof(idx_t));
        if (!tmp) abort();   /* allocation failure */
        cum = tmp;
        cum[cum_len++] = new_val;
    }
}

/* Find the row (0‑based) that contains the m‑th element (m is the index). */
static idx_t find_row(idx_t m) {
    /* Grow cum until it exceeds m */
    while (cum_len == 0 || cum[cum_len - 1] <= m) {
        ensure_cum_row((idx_t)cum_len);
    }
    /* Binary search for first cum[row] > m */
    idx_t lo = 0, hi = (idx_t)cum_len;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (cum[mid] > m)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;   /* row index */
}

seq_t a(idx_t n) {
    /* Determine row and column of the desired element */
    idx_t row = find_row(n);
    idx_t prev_cum = (row == 0) ? 0 : cum[row - 1];
    idx_t k = n - prev_cum;                     /* column within the row */

    /* Compute T(row,k) using the closed form: */
    /* T(r,k) = (-2)^(r-3k) * 3^k * C(r-2k, k) */
    idx_t p = row - 3 * k;                      /* exponent of (-2) */
    idx_t n2k = row - 2 * k;                    /* argument of binomial */

    seq_t bin = binomial(n2k, k);               /* binomial coefficient C(n-2k,k) */

    seq_t val = bin * pow3(k) * pow2(p);        /* multiply by 2^p and 3^k */

    if ((p & 1) != 0)                          /* apply the sign of (-1)^p */
        val = -val;

    return val;
}

/*
 * A265196  Irregular triangle read by rows, where T(n, k) is the coefficient
 *          of degree k of the polynomial Product_{j=0..n}
 *          (1-x^(3*j+1))*(1-x^(3*j+2)).
 * https://oeis.org/A265196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 72, i16 n <= 72, i32 n <= 72, i64 n <= 72, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A265196: the rows of
   the polynomial ∏_{j=0..r} (1 − x^(3j+1))(1 − x^(3j+2))
   flattened row‑wise.  For an index n, find the row r such that
   cumulative sums cum[r] ≤ n < cum[r+1], compute the coefficient
   of degree k = n − cum[r] in polynomial of row r, and return it. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static buffers for cumulative row counts and polynomial arrays.
   The buffers are grown lazily as needed.  They are shared between all
   invocations of a() and therefore preserve state across calls. */
static idx_t *cum = NULL;          /* cumulative counts: cum[0] = 0 */
static size_t cum_cap = 0;
static size_t cum_len = 0;          /* length of cum array ( == number of rows + 1 ) */

static seq_t *cur = NULL;          /* current polynomial coefficients */
static seq_t *next = NULL;         /* temporary polynomial coefficients */
static size_t buf_cap = 0;          /* capacity of cur/next arrays */

/* Ensure that the cumulative array covers index n. */
static void ensure_cum(idx_t n)
{
    if (cum_len == 0) {
        /* initialise with cum[0] = 0 */
        if (cum_cap < 1) {
            cum_cap = 8;
            cum = (idx_t*)malloc(sizeof(idx_t)*cum_cap);
        }
        cum[0] = 0;
        cum_len = 1;
    }
    while (cum_len > 0 && cum[cum_len-1] <= n) {
        /* compute row r = cum_len-1 (next row index) */
        idx_t r = cum_len - 1;
        idx_t lenr = (idx_t)(3 * ( (seq_t)(r+1) * (seq_t)(r+1) ) + 1);
        idx_t new_total = cum[cum_len-1] + lenr;
        if (cum_cap <= cum_len) {
            cum_cap = (cum_cap < 16) ? 16 : cum_cap * 2;
            cum = (idx_t*)realloc(cum, sizeof(idx_t)*cum_cap);
        }
        cum[cum_len] = new_total;
        cum_len++;
    }
}

/* Find the row index r such that cum[r] ≤ n < cum[r+1].
   cum_len must be > 1 and cum[cum_len-1] > n. */
static size_t find_row(idx_t n)
{
    size_t low = 0, high = cum_len - 2; /* last possible r is cum_len-2 */
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        if (cum[mid] <= n && n < cum[mid+1]) {
            return mid;
        } else if (n < cum[mid]) {
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    /* fallback (should not happen) */
    return 0;
}

/* Grow the polynomial buffers to at least 'need' elements. */
static void grow_poly(size_t need)
{
    if (buf_cap < need) {
        size_t new_cap = (buf_cap < 64) ? 64 : buf_cap * 2;
        while (new_cap < need) new_cap *= 2;
#if defined(__cplusplus)
        delete [] cur;
        delete [] next;
        cur = new seq_t[new_cap];
        next = new seq_t[new_cap];
#else
        cur = (seq_t*)realloc(cur, sizeof(seq_t)*new_cap);
        next = (seq_t*)realloc(next, sizeof(seq_t)*new_cap);
#endif
        buf_cap = new_cap;
    }
}

/* Compute the coefficient at degree 'k' of row 'r'. */
static seq_t row_coef(idx_t r, idx_t k)
{
    /* Final polynomial length for this row */
    size_t final_len = (size_t)(3 * ( (seq_t)(r+1) * (seq_t)(r+1) ) + 1);
    grow_poly(final_len);

    /* initialise first polynomial */
    for (size_t i = 0; i < final_len; ++i) cur[i] = 0;
    cur[0] = 1;
    size_t cur_len = 1;

    for (idx_t j = 0; j <= r; ++j) {
        idx_t a = 3*j + 1;
        idx_t b = 3*j + 2;
        size_t new_len = cur_len + a + b;

        /* zero next array up to new_len */
        for (size_t i = 0; i < new_len; ++i) next[i] = 0;

        /* accumulate contributions */
        for (size_t i = 0; i < cur_len; ++i) {
            seq_t coeff = cur[i];
            if (!coeff) continue;
            next[i] += coeff;
            next[i + a] -= coeff;
            next[i + b] -= coeff;
            next[i + a + b] += coeff;
        }

        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = new_len;
    }

    return cur[k];
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* quick shortcut for first term */

    ensure_cum(n);
    size_t r = find_row(n);
    idx_t k = n - cum[r];
    return row_coef((idx_t)r, k);
}

/*
 * A367209  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 1 + 4*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - x - x^2.
 * https://oeis.org/A367209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 42, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A367209:  Coefficients of the strong divisibility polynomials
 *  p(1,x)=1, p(2,x)=1+4x, p(n,x)=p(2,x)*p(n-1,x)+(1-x-x^2)*p(n-2,x)
 *  Read by rows (row 1: 1; row 2: 1,4; row 3: 2,7,15; ...).
 *  a(n) is the term obtained by stepping through the rows in order.
 *
 *  Formula (in recurrence form):
 *      a(1)=1, a(2)=1, a(3)=4,
 *      Let r be the smallest integer with r(r+1)/2 ≥ n, let
 *      c = n - r(r-1)/2  (1 ≤ c ≤ r).  Then
 *      a(n) = coeff of x^(c-1) in the polynomial p(r,x).
 *
 *  The algorithm computes the polynomials p(r,x) on demand, storing
 *  them in a static cache.  Each polynomial p(r,x) has degree r-1,
 *  so its coefficients are kept in an array of length r.
 *  Recursion:  p(r) = (1+4x)*p(r-1) + (1 - x - x^2)*p(r-2).
 *
 *  Complexity:  Each new row is built in O(r) time.  For n ≤ 10 000
 *  the maximum row index is 141, so the routine is trivial and
 *  responds in under 2 ms on a typical system.
 */

#include <stdlib.h>

static seq_t *alloc_row(size_t len)
{
    seq_t *p = (seq_t *)malloc(sizeof(seq_t) * len);
    if (!p) exit(EXIT_FAILURE);
    return p;
}

static idx_t triangular_row(idx_t n)
{
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;
    return r;
}

/*  Cache of computed polynomial coefficient rows.  */
static seq_t *row_cache[2000] = {0};

seq_t a(idx_t n)
{
    /*  Determine row index r and column offset  */
    idx_t r = triangular_row(n);
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t offset   = n - prev_sum - 1;   /* 0‑based index  */

    /*  Ensure rows 1 and 2 are initialized  */
    if (row_cache[1] == NULL) {
        row_cache[1] = alloc_row(1);
        row_cache[1][0] = 1;
    }
    if (row_cache[2] == NULL) {
        row_cache[2] = alloc_row(2);
        row_cache[2][0] = 1;
        row_cache[2][1] = 4;
    }

    /*  Compute missing rows up to r  */
    for (idx_t k = 3; k <= r; k++) {
        if (row_cache[k] != NULL) continue;   /* already computed */
        size_t len = (size_t)k;
        seq_t *new_row = alloc_row(len);
        for (size_t j = 0; j < len; j++) new_row[j] = 0;

        seq_t *prev1 = row_cache[k - 1];
        seq_t *prev2 = row_cache[k - 2];

        /*  Contribution from (1+4x)*prev1  */
        for (size_t j = 0; j < k - 1; j++) {
            new_row[j]   += prev1[j];          /* *1  */
            new_row[j+1] += prev1[j] * 4;      /* *4x */
        }

        /*  Contribution from (1 - x - x^2)*prev2  */
        for (size_t j = 0; j < k - 2; j++) {
            new_row[j]   += prev2[j];          /* *1   */
            new_row[j+1] -= prev2[j];          /* *-1x */
            new_row[j+2] -= prev2[j];          /* *-1x^2 */
        }

        row_cache[k] = new_row;
    }

    return row_cache[r][offset];
}

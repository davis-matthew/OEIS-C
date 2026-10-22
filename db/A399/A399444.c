/*
 * A399444  Triangle read by rows: T(n,k) is the number of complete
 *          non-ambiguous trees with n+1 leaves and maximum weight 0, refined
 *          by the degree k of the minimal vertex of the associated fully
 *          tiered tree.
 * https://oeis.org/A399444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(m,k) where m is the largest integer with m(m+1)/2 ≤ n
 * and k = n – m(m+1)/2.  For triangular n (k = 0) a(n) = 0.  T(n,k) is
 * defined by:
 *
 *   T(0,0) = 1,   T(n,0) = 0 for n>0
 *   T(n,1) = Σ_{j=0}^{n-1} T(n-1,j)
 *   T(n,k) = Σ_{i=1}^{n-k+1} T(n-i,k-1) *
 *            binomial(n,i) * binomial(n-k,i-1) *
 *            Σ_{j=0}^{i-1} T(i-1,j)   for k ≥ 2
 *
 * The function below memoises the triangular array T(n,k) for all
 * needed n and k, computing binomials with the helper from utils.
 */

#include <stdlib.h>
#include "../utils/binom.h"        /* seq_t binom(idx_t n, idx_t k); */

static seq_t **triangle = NULL;      /* triangle[n][k]            */
static size_t triangle_cap = 0;      /* capacity of triangle array */
static size_t triangle_count = 0;    /* rows currently stored     */

static seq_t *row_sums = NULL;      /* sum_{k} T(n,k) per row   */
static size_t row_sums_cap = 0;
static size_t row_sums_count = 0;

/* Ensure triangle is computed up to row m (0‑based). */
static void ensure_triangle(idx_t m)
{
    /* extend triangle pointer array if required */
    if (triangle_count <= (size_t)m) {
        size_t new_cap = (size_t)m + 1;
        seq_t **new_tri = (seq_t**)malloc(new_cap * sizeof(seq_t*));
        if (!new_tri) return;                /* allocation failure - abort */
        if (triangle) {
            memcpy(new_tri, triangle, triangle_count * sizeof(seq_t*));
            free(triangle);
        }
        for (size_t i = triangle_count; i < new_cap; ++i)
            new_tri[i] = NULL;
        triangle = new_tri;
        triangle_cap = new_cap;
    }

    /* extend row_sums array if required */
    if (row_sums_count <= (size_t)m) {
        size_t new_cap = (size_t)m + 1;
        seq_t *new_sums = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_sums) return;
        if (row_sums) {
            memcpy(new_sums, row_sums, row_sums_count * sizeof(seq_t));
            free(row_sums);
        }
        for (size_t i = row_sums_count; i < new_cap; ++i)
            new_sums[i] = (seq_t)0;
        row_sums = new_sums;
        row_sums_cap = new_cap;
    }

    /* compute rows from triangle_count up to m */
    for (idx_t r = triangle_count; r <= (idx_t)m; ++r) {
        seq_t *row = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!row) return;
        for (idx_t j = 0; j <= r; ++j)
            row[j] = (seq_t)0;

        if (r == 0) {                    /* base row */
            row[0] = (seq_t)1;
            row_sums[0] = row[0];
            triangle[0] = row;
            continue;
        }

        /* T(r,0) is 0 for r>0 */
        /* T(r,1) = sum_{k=0}^{r-1} T(r-1,k) = row_sums[r-1] */
        row[1] = row_sums[r - 1];
        seq_t sum_row = row[0] + row[1];

        /* compute T(r,k) for k=2..r */
        for (idx_t k = 2; k <= r; ++k) {
            seq_t term_sum = (seq_t)0;
            for (idx_t i = 1; i <= r - k + 1; ++i) {
                seq_t part1 = triangle[r - i][k - 1];
                seq_t part2 = binom(r, i);
                seq_t part3 = binom(r - k, i - 1);
                seq_t part4 = row_sums[i - 1];
                seq_t term = part1 * part2;
                term = term * part3;
                term = term * part4;
                term_sum = term_sum + term;
            }
            row[k] = term_sum;
            sum_row = sum_row + term_sum;
        }

        row_sums[r] = sum_row;
        triangle[r] = row;
    }

    triangle_count = (size_t)m + 1;
    row_sums_count = (size_t)m + 1;
}

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                        /* a(0) = 1 */

    /* Find m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2 */
    idx_t m = 0;
    while (((m + 1) * (m + 2)) / 2 <= n)
        ++m;

    /* Triangular cases produce 0 */
    if (m * (m + 1) / 2 == n)
        return (seq_t)0;

    idx_t k = n - m * (m + 1) / 2;            /* 1 ≤ k ≤ m */

    ensure_triangle(m);                       /* compute needed rows */
    return triangle[m][k];
}

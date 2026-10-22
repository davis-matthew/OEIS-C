/*
 * A322670  Number T(n,k) of colored set partitions of [n] where colors of the
 *          elements of subsets are distinct and in increasing order and
 *          exactly k colors are used; triangle T(n,k), n>=0, 0<=k<=n, read by
 *          rows.
 * https://oeis.org/A322670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major flattening of T(row,col) * where T(n,k)
 *            counts colored set partitions of [n] with k colors * (colors of
 *            elements in each block distinct and increasing). * a(n) = T(row,
 *            col) with * row = max r with r(r+1)/2 <= n * col = n -
 *            row(row+1)/2 * * The values are obtained by first c...
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 59, u64 n <= 128,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment.  */
extern seq_t binomCoeff(idx_t n, idx_t k);  /* unused – we compute ours */
extern seq_t binomCoeffMut(idx_t n, idx_t k);  /* unused – we compute ours */

#define MAX_N 205   /* sufficient for all n ≤ 10000 (row ≤ 140) */

static seq_t binom[MAX_N][MAX_N];
static seq_t A[MAX_N][MAX_N];
static bool tables_built = false;
static idx_t tables_built_to = 0;

/* --------------------------------------------------------------- */
/* Compute binomial coefficients C(n,k) for 0 ≤ n,k < MAX_N        */
/* Using Pascal's rule to avoid divisions.                        */
/* --------------------------------------------------------------- */
static void build_binom(void)
{
    for (idx_t n = 0; n < MAX_N; ++n) {
        binom[n][0] = 1;
        binom[n][n] = 1;
        for (idx_t k = 1; k < n; ++k) {
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }
}

/* --------------------------------------------------------------- */
/* Compute A[n][s] for all 0 ≤ n,s < MAX_N.                       */
/* A[n][s] = number of colored set partitions of [n] that use at most s colors. */
/* --------------------------------------------------------------- */
static void build_tables(void)
{
    if (tables_built) return;

    build_binom();  /* binomial coefficients needed */

    /* Base case: n = 0 */
    for (idx_t s = 0; s < MAX_N; ++s)
        A[0][s] = 1;

    /* Recurrence for n > 0 */
    for (idx_t n = 1; n < MAX_N; ++n) {
        A[n][0] = 0;   /* no colors can cover any element */
        for (idx_t s = 1; s < MAX_N; ++s) {
            seq_t sum = 0;
            /* r = size of block containing element “n” */
            idx_t max_r = (n < s) ? n : s;
            for (idx_t r = 1; r <= max_r; ++r) {
                /* choose r-1 other elements from the first n-1 ones */
                seq_t ways_choose = binom[n-1][r-1];
                /* choose r distinct colors for that block */
                seq_t ways_color = binom[s][r];
                /* ways to color the remaining n-r elements with at most s colors */
                seq_t ways_rest = A[n-r][s];
                sum += ways_choose * ways_color * ways_rest;
            }
            A[n][s] = sum;
        }
    }
    tables_built = true;
    tables_built_to = MAX_N - 1;
}

/* --------------------------------------------------------------- */
/* Compute T(n,k) using inclusion–exclusion.                       */
/* --------------------------------------------------------------- */
static seq_t T_value(idx_t n, idx_t k)
{
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t s = 0; s <= k; ++s) {
        seq_t term = binom[k][s] * A[n][s];
        if ((k - s) & 1) {
            neg += term;
        } else {
            pos += term;
        }
    }
    return pos - neg;  /* pos ≥ neg for all valid (n,k) */
}

/* --------------------------------------------------------------- */
/* Helper: find row and col for linear index n                    */
/* --------------------------------------------------------------- */
static void index_to_rowcol(idx_t linear, idx_t *row, idx_t *col)
{
    /* Find max r with r(r+1)/2 <= linear */
    idx_t r = 0;
    /* linear <= 10000, so simple loop is fine */
    while ((r + 1) * (r + 2) / 2 <= linear)
        ++r;
    *row = r;
    *col = linear - r * (r + 1) / 2;
}

/* --------------------------------------------------------------- */
/* Public function required by the problem statement              */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_rowcol(n, &row, &col);
    /* Ensure tables cover this row/col */
    if (row >= MAX_N)
        /* If n is beyond pre‑computed range, grow the tables.
           For the current problem constraints this case never occurs. */
        return 0;
    build_tables();
    return T_value(row, col);
}

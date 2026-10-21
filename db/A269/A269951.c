/*
 * A269951  Triangle read by rows, T(n,k) = Sum_{j=0..n}
 *          (-1)^(n-j)*C(-j,-n)*S1(j,k), S1 the Stirling cycle numbers
 *          A132393, for n>=0 and 0<=k<=n.
 * https://oeis.org/A269951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(r,c) = sum_{j=1..r} binomial(r-1,j-1) * S1(j,c)
 *            with S1 the unsigned Stirling numbers of the first kind
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t **stirling = NULL;          /* S1[j][k] stored for j up to stirling_rows-1 */
static idx_t   stirling_rows = 0;

static seq_t **T_cache = NULL;           /* triangle values T(r,k) for rows up to T_rows-1 */
static bool    *T_computed = NULL;
static idx_t   T_rows       = 0;

/* Ensure Stirling numbers up to n (inclusive) are computed */
static void ensure_stirling(idx_t n)
{
    if (n < stirling_rows)
        return;

    /* Reallocate array of pointers for new rows */
    seq_t **new_ptr = (seq_t **)realloc(stirling, (n + 1) * sizeof(seq_t *));
    if (!new_ptr)
        exit(1);
    stirling = new_ptr;

    /* Allocate arrays for new rows and initialize to zero */
    for (idx_t j = stirling_rows; j <= n; ++j)
    {
        stirling[j] = (seq_t *)calloc(j + 1, sizeof(seq_t));
        if (!stirling[j])
            exit(1);
    }

    /* Build Stirling numbers incrementally */
    if (stirling_rows == 0)                 /* row 0 */
    {
        stirling[0][0] = (seq_t)1;
    }
    for (idx_t j = stirling_rows; j <= n; ++j)
    {
        if (j == 0)
            continue;
        stirling[j][0] = 0;
        /* S1(j,j) = 1 */
        stirling[j][j] = (seq_t)1;
        for (idx_t k = 1; k < j; ++k)
        {
            seq_t term1 = stirling[j - 1][k - 1];
            seq_t term2 = stirling[j - 1][k];
            stirling[j][k] = term1 + (term2 * (seq_t)(j - 1));
        }
    }

    stirling_rows = n + 1;
}

/* Ensure we have storage for triangle rows up to n (inclusive) */
static void ensure_row_storage(idx_t n)
{
    if (n < T_rows)
        return;

    seq_t **new_ptr = (seq_t **)realloc(T_cache, (n + 1) * sizeof(seq_t *));
    bool    *new_f  = (bool *)realloc(T_computed, (n + 1) * sizeof(bool));
    if (!new_ptr || !new_f)
        exit(1);

    T_cache     = new_ptr;
    T_computed  = new_f;

    for (idx_t i = T_rows; i <= n; ++i)
    {
        T_cache[i]    = NULL;
        T_computed[i] = false;
    }

    T_rows = n + 1;
}

/* Compute all entries of row r and store them */
static void compute_row(idx_t r)
{
    if (T_computed[r])
        return;

    if (r == 0)
    {
        T_cache[0] = (seq_t *)malloc(sizeof(seq_t));
        T_cache[0][0] = (seq_t)1;
        T_computed[0] = true;
        return;
    }

    T_cache[r] = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    for (idx_t k = 0; k <= r; ++k)
    {
        seq_t sum = 0;
        for (idx_t j = 1; j <= r; ++j)
        {
            if (k > j)
                continue;
            seq_t bin = binomial(r - 1, j - 1);
            seq_t prod = stirling[j][k] * bin;
            sum += prod;
        }
        T_cache[r][k] = sum;
    }
    T_computed[r] = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    ensure_stirling(r);
    ensure_row_storage(r);
    compute_row(r);

    return T_cache[r][c];
}

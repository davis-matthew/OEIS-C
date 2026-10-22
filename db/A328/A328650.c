/*
 * A328650  Triangular array read by rows: row n shows the coefficients of
 *          this polynomial of degree n: (1/n!)*(numerator of n-th derivative
 *          of 1)/(1-x-2x^2).
 * https://oeis.org/A328650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 40, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A328650
 *
 *  a(n) is obtained from the triangular array whose row r (r ≥ 0)
 *  consists of the coefficients of the polynomial P_r(x) of degree r:
 *
 *      P_0(x) = 1
 *      P_{m+1}(x) = ((1 - x - 2x^2) * P_m'(x) + (m+1) * P_m(x) * (1 + 4x)) / (m+1)
 *
 *  For a given index n let r be the unique integer with
 *      r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *  and let k = n - r(r+1)/2.
 *  Then
 *
 *      a(n) = (-1)^{r+1} * coefficient of x^k in P_r(x).
 *
 *  The implementation builds the requested row of polynomials only
 *  as needed, caching all earlier rows.  The algorithm is O(r^2)
 *  per call, which is easily fast enough for n up to 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The integer types seq_t, idx_t are supplied by the build system. */
extern seq_t a(idx_t n);

/* Allocate or grow the global cache of rows.  Each row is an array
 * of seq_t of length row_index+1.  The cache grows geometrically. */
static seq_t **row_cache = NULL;
static idx_t  cache_cap  = 0;     /* number of pointers currently allocated           */
static idx_t  cache_last = 0;     /* highest index that has been constructed (0‑based) */

/* Compute the smallest m such that m(m+1)/2 > n, then r = that-1.
   For small n an incrementing loop is fine; use __int128 to avoid overflow. */
static idx_t triangular_row(idx_t n)
{
    idx_t r = 0;
    while ((((unsigned __int128)(r + 1) * (r + 2)) / 2) <= n) ++r;
    return r;
}

/* Build rows from cache_last+1 up to target_r inclusive */
static void build_rows_upto(idx_t target_r)
{
    for (idx_t i = cache_last + 1; i <= target_r; ++i)
    {
        /* Previous row */
        idx_t prev_index = i - 1;
        seq_t *prev     = row_cache[prev_index];
        idx_t prev_len  = prev_index + 1;        /* length of previous row */

        /* Current row length i+1 */
        idx_t curr_len  = i + 1;
        size_t curr_len_s = (size_t)curr_len;

        seq_t *curr = (seq_t *)calloc(curr_len_s, sizeof(seq_t));
        seq_t *tmpA = (seq_t *)calloc(curr_len_s, sizeof(seq_t));

        /* Derivative part: g(x) * P_{i-1}'(x) with g(x)=1-x-2x^2 */
        for (idx_t k = 0; k < prev_len - 1; ++k)
        {
            seq_t dcoeff = (seq_t)(k + 1) * prev[(size_t)(k + 1)];
            tmpA[(size_t)k]           += dcoeff;
            tmpA[(size_t)k + 1]       -= dcoeff;
            tmpA[(size_t)k + 2]       -= (seq_t)2 * dcoeff;
        }

        /* Divide contribution of derivative by (i) */
        seq_t denom = (seq_t)i;
        for (size_t j = 0; j < curr_len_s; ++j) tmpA[j] = tmpA[j] / denom;

        /* Add the (i) * P_{i-1}(x) * (1 + 4x) part */
        for (size_t j = 0; j < curr_len_s; ++j)
        {
            seq_t part = 0;
            if (j < prev_len)                        part += prev[(size_t)j];
            if (j >= 1 && (j - 1) < prev_len)        part += (seq_t)4 * prev[(size_t)j - 1];
            curr[j] = tmpA[j] + part;
        }

        free(tmpA);

        /* Ensure the cache can hold row i */
        if (cache_cap <= i)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= (size_t)i) new_cap *= 2;
            seq_t **tmp = (seq_t **)realloc(row_cache, new_cap * sizeof(seq_t *));
            row_cache = tmp;
            cache_cap = (idx_t)new_cap;
        }

        row_cache[i] = curr;
        cache_last   = i;
    }
}

seq_t a(idx_t n)
{
    /* Determine row r and column k for the triangular index n */
    idx_t r = triangular_row(n);
    idx_t k = n - r * (r + 1) / 2;

    /* Initialise cache with row 0 if necessary */
    if (!row_cache)
    {
        cache_cap   = 1;
        row_cache   = (seq_t **)malloc(cache_cap * sizeof(seq_t *));
        seq_t *row0 = (seq_t *)calloc(1, sizeof(seq_t));
        row0[0]     = 1;          /* P_0(x) = 1 */
        row_cache[0] = row0;
        cache_last  = 0;
    }

    /* Build rows up to r if not yet present */
    if (cache_last < r) build_rows_upto(r);

    /* Retrieve coefficient and apply overall sign */
    seq_t coeff = row_cache[r][k];
    if ((r & 1) == 0)      /* (-1)^{r+1} = -1 when r even */
        coeff = -coeff;

    return coeff;
}

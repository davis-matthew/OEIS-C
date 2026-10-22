/*
 * A382232  Irregular triangle read by rows: T(n,k) = [x^k] (1+x) * A_n(x)^2,
 *          where A_n(x) is the n-th Eulerian polynomial.
 * https://oeis.org/A382232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 35, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A382232:
   a(n) is the nth entry of the irregular triangle read by rows,
   where the triangle is defined by
        T(n,k) = [x^k] (1+x) * A_n(x)^2
   and A_n(x) is the nth Eulerian polynomial.
   The triangle rows have lengths:
        row 0: 2 entries
        row 1: 2 entries
        row n (n ≥ 2): 2*n entries
   The sequence is the row‑by‑row concatenation of the triangle. */

/* Note: only the function seq_t a(idx_t n) is required.
   Helper functions and headers are provided by the build system.  */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"

/* Integer square‑root for idx_t values (floor of sqrt).  No floating point. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n < 2u) return n;
    idx_t low = 0;
    idx_t high = (n >> 1u) + 1u;
    while (low < high)
    {
        idx_t mid = (low + high) / 2u + 1u;  /* ceil */
        if (mid > 0 && mid > n / mid)
            high = mid - 1u;
        else
            low = mid;
    }
    return low;
}

/* ---------- Eulerian rows cache --------------------------------- */
static seq_t **euler_rows = NULL;      /* euler_rows[r] -> array of length max(1,r) */
static size_t euler_row_cnt = 0;       /* number of rows stored so far */
static size_t euler_row_cap = 0;

/* Grow the pointer array and generate missing Eulerian rows up to 'rows_needed' (inclusive). */
static void ensure_euler_rows(idx_t rows_needed)
{
    if ((idx_t)euler_row_cnt > rows_needed)
        return;

    /* Allocate or grow the pointer array */
    if (euler_row_cnt == 0)      /* first use */
    {
        euler_row_cap = 8;
        euler_rows = (seq_t**)calloc(euler_row_cap, sizeof(seq_t *));
    }
    else if (rows_needed >= euler_row_cap)
    {
        size_t new_cap = euler_row_cap * 2;
        while (new_cap <= (size_t)rows_needed) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(euler_rows, new_cap * sizeof(seq_t *));
        euler_rows = tmp;
        euler_row_cap = new_cap;
    }

    /* Compute each missing row */
    for (idx_t r = euler_row_cnt; r <= rows_needed; ++r)
    {
        size_t len = (r == 0u) ? 1u : (size_t)r;     /* length of A_r(x) coefficients */
        seq_t *row = seq_alloc_new_array(len);       /* construct array of seq_t */
        row[0] = (seq_t)1;                           /* A_0(x)=1, A_1(x)=1 */

        if (r >= 1u)
        {
            seq_t *prev = euler_rows[r - 1u];
            size_t prev_len = (r == 1u) ? 1u : (size_t)(r - 1u);

            for (size_t k = 0; k < len; ++k)
            {
                seq_t left  = (k < prev_len) ? prev[k] : (seq_t)0;
                seq_t right = (k > 0u) ? prev[k - 1] : (seq_t)0;
                row[k] = ((seq_t)(k + 1u)) * left + ((seq_t)(r - k)) * right;
            }
        }
        euler_rows[r] = row;
    }
    euler_row_cnt = rows_needed + 1u;
}

/* ---------- Compute a single triangle entry T(r,k) --------------------- */
static seq_t compute_T(idx_t r, idx_t k)
{
    seq_t *a = euler_rows[r];
    size_t len = (r == 0u) ? 1u : (size_t)r;
    seq_t t1 = (seq_t)0;   /* coefficient of x^k in A_r(x)^2 */
    seq_t t2 = (seq_t)0;   /* coefficient of x^(k-1) in A_r(x)^2 */

    for (size_t i = 0; i < len; ++i)
    {
        idx_t j = k - (idx_t)i;
        if (j < (idx_t)len)
            t1 += a[i] * a[(size_t)j];
    }
    if (k > 0u)
    {
        for (size_t i = 0; i < len; ++i)
        {
            idx_t j = k - 1u - (idx_t)i;
            if (j < (idx_t)len)
                t2 += a[i] * a[(size_t)j];
        }
    }
    return t1 + t2;
}

/* ---------- Main sequence accessor ------------------------------------- */
seq_t a(idx_t n)
{
    /* Row 0 contains the first two outputs. */
    if (n < 2u)
        return (seq_t)1;

    /* Compute r such that cumulative(r-1) <= n < cumulative(r)
       where cumulative(r) = r*(r+1)+2. */
    idx_t t = n - 2u;              /* t = n-2  (t >= 0) */
    idx_t val = 4u * t + 1u;        /* val = 4*t + 1 */
    idx_t sq  = floor_isqrt_u64(val); /* sqrt floor */
    idx_t r   = (sq - 1u) / 2u;     /* candidate r */
    if (r * (r + 1u) <= t)
        ++r;                         /* ensure cumulative(r) > n */

    /* Ensure Eulerian rows up to r are cached. */
    ensure_euler_rows(r);

    /* Compute k within the chosen row. */
    idx_t cumulative_prev = (r == 0u ? 2u : (idx_t)(r - 1u) * r + 2u);
    idx_t k = n - cumulative_prev;

    return compute_T(r, k);
}

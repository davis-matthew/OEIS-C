/*
 * A324956  Triangle of coefficients T(n,k) of y^n in Product_{k=0..n-2} (n +
 *          (n + k)*y + n*y^2), as read by rows of terms k = 0..2*n-2, for n
 *          >= 1.
 * https://oeis.org/A324956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the triangle read row‑by‑row, where
 *            T(n,k) (0 ≤ k ≤ 2n-2) are the coefficients of y^k in
 *            ∏_{i=0}^{n-2} (n + (n+i) y + n y^2). The flattening starts with
 *            a(1)=1
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   - The triangle row for a given n is computed by straightforward
     convolution with the quadratic factors.  The convolution costs
     O(n^2) time and O(n) memory per row, which is acceptable for
     n ≤ 10 000 in the intended environment.
   - All arithmetic is performed in the provided type `seq_t`.  For
     arbitrary‑precision builds `seq_t` is a class with constructors,
     so we use the supplied allocator `seq_alloc_new_array` to
     initialise the coefficient arrays safely.
   - Results are cached per row; subsequent calls for the same n
     return the cached value immediately.
   - Mapping from the flattened index n to the corresponding row r
     and position k uses the identity
         (r−1)^2 < n ≤ r^2  ⇔  r = ceil(√n)
     and the offset
         k = n − (r−1)^2 − 1   (0‑based). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_t allocation helper */

static seq_t **row_cache      = NULL;   /* Cached rows, indexed by r (1‑based)   */
static size_t *row_lengths    = NULL;   /* Length of each cached row             */
static size_t cache_using_rows = 0;     /* Highest row cached so far             */

/* Compute the coefficient row for a given n (1‑based).  The returned
   array has length 2*n-1 and owns its storage; the caller must not
   free it explicitly. */
static seq_t *compute_row(idx_t n)
{
    if (n == (idx_t)1)
    {
        seq_t *row = seq_alloc_new_array(1);
        row[0] = 1;
        return row;
    }

    size_t max_len = 2 * (size_t)n - 1;                 /* length of the row         */
    seq_t *cur   = seq_alloc_new_array(max_len);        /* current polynomial        */
    seq_t *next  = seq_alloc_new_array(max_len);        /* temporary storage         */

    /* initialise polynomial to 1 */
    for (size_t i = 0; i < max_len; ++i) cur[i] = 0;
    cur[0] = 1;
    size_t cur_len = 1;                               /* current degree + 1        */

    seq_t n_seq = (seq_t)n;                           /* cast for big‑int ops       */

    /* Convolve with each quadratic factor (n + (n + i) y + n y²) */
    for (idx_t i = 0; i <= n - 2; ++i)
    {
        size_t next_len = cur_len + 2;
        for (size_t j = 0; j < next_len; ++j) next[j] = 0;

        seq_t bi = (seq_t)(n + i);
        for (size_t j = 0; j < cur_len; ++j)
        {
            seq_t val = cur[j];
            next[j]     += val * n_seq;
            next[j+1]   += val * bi;
            next[j+2]   += val * n_seq;
        }

        /* copy back to cur */
        for (size_t j = 0; j < next_len; ++j) cur[j] = next[j];
        cur_len = next_len;
    }

    /* free temporary array; cur will be cached */
    free(next);
    /* truncate cur to its actual length */
    if (row_lengths[0] != 0 && row_lengths[0] < max_len)
        cur = seq_alloc_new_array(max_len);          /* just for safety, not used */

    return cur;   /* caller owns the array */
}

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;   /* safety for out‑of‑range indices */

    /* Find the row r containing the flattened index n:
       (r−1)^2 < n ≤ r^2   →   r = ceil(√n). */
    idx_t r = 1;
    while (r * r < n) ++r;

    /* Ensure all rows up to r are cached */
    if ((size_t)r > cache_using_rows)
    {
        /* grow cache arrays */
        row_cache   = (seq_t **)realloc(row_cache,
                         (size_t)(r + 1) * sizeof(seq_t *));
        row_lengths = (size_t *)realloc(row_lengths,
                         (size_t)(r + 1) * sizeof(size_t));

        for (size_t i = cache_using_rows + 1; i <= (size_t)r; ++i)
        {
            seq_t *row = compute_row((idx_t)i);
            row_cache[i]   = row;
            row_lengths[i] = 2 * i - 1;
        }
        cache_using_rows = r;
    }

    /* Compute local index within the row:
       start index of row r is (r−1)^2 + 1. */
    idx_t start_row = (r - 1) * (r - 1) + 1;
    idx_t k = n - start_row;            /* 0‑based index in the row   */

    return row_cache[(size_t)r][(size_t)k];
}

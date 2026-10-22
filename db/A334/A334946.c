/*
 * A334946  Irregular triangle read by rows: T(n,k) is the number of
 *          partitions of n into k consecutive parts that differ by 6, and the
 *          first element of column k is in the row that is the k-th octagonal
 *          number (A000567).
 * https://oeis.org/A334946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A334946:
   Irregular triangle read by rows: T(n,k) = 1 if n can be written
   as a sum of k consecutive integers that differ by 6, otherwise 0.
   The triangle is flattened row by row and a(n) returns the
   n‑th entry (1‑based).  The algorithm generates complete rows until
   the required index is reached. */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of flattened triangle entries (0 or 1). */
static seq_t *cache       = NULL;   /* flattened values */
static size_t cache_cap   = 0;      /* allocated capacity in seq_t */
static idx_t  cache_len   = 0;      /* number of values stored */
static idx_t  next_row    = 1;      /* next row (n) to generate */

/* Grow the cache so that it can hold at least needed elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the largest k such that k*(3*k-2) <= r.  Simple linear search
   is sufficient because k is O(sqrt(r)) and r will be small (≤ few
   thousand rows for n ≤ 10000). */
static idx_t max_k(idx_t r)
{
    idx_t k = 1;
    while (k * (3*k - 2) <= r)
        ++k;
    return k - 1;
}

/* Generate the flattened triangle up to at least target index
   (1‑based).  The whole rows are generated so the mapping between
   flattened index and (row, column) stays correct. */
static void generate_up_to(idx_t target)
{
    while (cache_len < target)
    {
        idx_t r = next_row;          /* current row number (n)   */
        idx_t kmax = max_k(r);       /* number of columns in this row |
                                      *  (columns with O_k ≤ r)      */

        for (idx_t kk = 1; kk <= kmax; ++kk)
        {
            /* T(r,kk) = 1 iff (r - 3*kk*(kk-1)) is divisible by kk
               and the resulting a = (r - 3*kk*(kk-1))/kk >= 1. */
            seq_t diff = (seq_t)r - (seq_t)3 * kk * (kk - 1);
            seq_t val = ((diff % kk) == 0 && diff / kk >= (seq_t)1) ? 1 : 0;

            /* Append to cache. */
            cache_len++;
            grow_cache(cache_len + 1);   /* ensure room for new value */
            cache[cache_len - 1] = val;
        }
        next_row++;
    }
}

/* Public function: returns the n‑th term of A334946 (1‑based).   */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, return 0 */
    if (n > cache_len)
        generate_up_to(n);              /* extend cache to n */
    return cache[n - 1];
}

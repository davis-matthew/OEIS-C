/*
 * A390369  Triangle read by rows: T(n,0) = T(n,n) = 0 and T(n,k) = T(n, k-1)
 *          + T(n-1, k-1) + T(n-2, k-1) + 1 for 0 < k < n.
 * https://oeis.org/A390369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) computed by building the triangle T(n,k) * with
 *            T(n,0)=T(n,n)=0, T(n,k)=T(n,k-1)+T(n-1,k-1)+T(n-2,k-1)+1 * and
 *            then reading the triangle row‑by‑row. The function * maintains a
 *            cache of all computed entries so that even * repeated queries
 *            are fast, and it works for any type seq_t
 * Range:     u8 n <= 41, u16 n <= 102, u32 n <= 321, u64 n <= 1032,
 *            u128 n <= 3737, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"

/* static cache of all already computed coefficients */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;          /* number of cached entries */
static idx_t  last_row_done = (idx_t)-1;  /* highest row that has been fully computed */

/* external‑linkage function required by the judge */
seq_t a(idx_t n)
{
    /* If the requested index is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Compute the row number r that contains index n. */
    double d = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Correct round‑off errors that may occur with sqrt. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n)       --r;

    /* Number of entries needed up to and including row r. */
    idx_t needed = (r + 2) * (r + 1) / 2;

    /* Grow the cache if necessary using the provided allocator. */
    if (needed > cache_size) {
        seq_t *tmp = alloc_seq_array(needed);
        if (!tmp)            /* allocation failed – return zero (unlikely) */
            return 0;

        /* Copy the old cached values. */
        if (cache) {
            idx_t i;
            for (i = 0; i < cache_size; ++i)
                tmp[i] = cache[i];
        }
        cache = tmp;
        cache_size = needed;
    }

    /* Compute rows that have not yet been computed. */
    idx_t start_row = (last_row_done == (idx_t)-1) ? 0 : last_row_done + 1;

    for (idx_t row = start_row; row <= r; ++row) {
        idx_t row_start = row * (row + 1) / 2;   /* first global index of this row */

        for (idx_t k = 0; k <= row; ++k) {
            idx_t idx = row_start + k;

            if (k == 0 || k == row) {
                /* Boundary values are zero. */
                cache[idx] = 0;
            } else {
                /* Recurrence:
                 * T(n,k) = T(n,k-1) + T(n-1,k-1) + T(n-2,k-1) + 1 */
                seq_t val = cache[idx - 1];                 /* T(n,k-1)      */
                val += cache[idx - (row + 1)];              /* T(n-1,k-1)    */
                val += cache[idx - 2 * row];                /* T(n-2,k-1)    */
                val += 1;
                cache[idx] = val;
            }
        }
    }

    /* Remember the highest row that has been computed. */
    last_row_done = r;
    return cache[n];
}

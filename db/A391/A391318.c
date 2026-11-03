/*
 * A391318  Triangle array read by rows: T(n,k) = T(n-1,k) + T(n-1,k-1) +
 *          T(n-2,k-1) + 1 with T(0,0) = T(n,-1) = T(n,n+1) = 0.
 * https://oeis.org/A391318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 110, u32 n <= 389, u64 n <= 1453,
 *            u128 n <= 5150, big any n
 * Validated: standard, pass, 82 terms through n = 5035 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k-1) + 1,
 * with T(0,0)=0; a(n) is the n-th element of the flattened triangle
 * read by rows. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* a(0) is trivial. */
    if (n == 0) return (seq_t)0;

    /* Determine the row r that contains the index n. */
    idx_t r = 0;
    while (n >= ((idx_t)(r + 1) * (r + 2)) / 2) ++r;

    /* Column k within that row. */
    idx_t k = n - (idx_t)((seq_t)r * (r + 1) / 2);

    /* Size needed for all rows up to r. */
    idx_t maxLen = r + 1;

    /* Allocate three working rows, each long enough for the longest. */
    seq_t *prevPrev = mem_new(seq_t, (size_t)maxLen);
    seq_t *prev     = mem_new(seq_t, (size_t)maxLen);
    seq_t *cur      = mem_new(seq_t, (size_t)maxLen);

    /* Row 0 (T(0,0)=0) already present in 'prev' due to calloc. */
    /* Iterate from row 1 up to row r. */
    for (idx_t i = 1; i <= r; ++i) {
        size_t len = (size_t)(i + 1);
        for (size_t j = 0; j < len; ++j) {
            seq_t val = (seq_t)1;                 /* constant term */
            if (j < (size_t)i)                    /* T(i-1,j)   */
                val += prev[j];
            if (j > 0)                            /* T(i-1,j-1) */
                val += prev[j - 1];
            if (i >= 2 && j > 0)                  /* T(i-2,j-1) */
                val += prevPrev[j - 1];
            cur[j] = val;
        }
        /* Rotate the row buffers for the next iteration. */
        seq_t *tmp = prevPrev;
        prevPrev  = prev;
        prev      = cur;
        cur       = tmp;
    }

    seq_t result = prev[(size_t)k];

    mem_free(prevPrev);
    mem_free(prev);
    mem_free(cur);

    return result;
}

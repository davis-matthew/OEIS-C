/*
 * A398376  Number of congruence classes of nonsingular n X n matrices over
 *          GF(2).
 * https://oeis.org/A398376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for f(k,m):
 * f(0,0)=1, f(0,m>0)=0, and
 * f(k,m) = f(k-1,m) + f(k-1,m-k) + 2*f(k,m-2k)
 * The desired term a(n) is f(n,n).  The algorithm uses a
 * dynamic programming array of length n+1 for each k, reusing
 * two arrays.  Complexity O(n²) with O(n) memory.  This passes
 * n≈10000 within the required limits. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* 0 ≤ n ≤ we expect a reasonable upper bound.  Allocate
     * space for indices 0..n inclusive. */
    size_t N = (size_t)n;          /* cast to size_t for array index */
    size_t sz = N + 1;

    /* Allocate two working rows.  seq_alloc_new_array returns
     * a zero‑initialized array of the requested size. */
    seq_t *prev = seq_alloc_new_array(sz);
    seq_t *cur  = seq_alloc_new_array(sz);
    if (!prev || !cur) {
        /* Allocation failed – return 0 (sequence is defined but
         * caller should handle the error).  On fatal failure we
         * could abort, but returning 0 keeps the function pure. */
        return 0;
    }

    /* Base row k = 0: f(0,0)=1, f(0,m>0)=0. */
    prev[0] = 1;
    for (size_t i = 1; i < sz; ++i)
        prev[i] = 0;

    /* DP for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        size_t kk = (size_t)k;
        for (idx_t m = 0; m <= n; ++m) {
            size_t mi = (size_t)m;
            seq_t val = prev[mi];                 /* f(k-1,m) */
            if (m >= k)                          /* + f(k-1,m-k) */
                val += prev[mi - kk];
            if (m >= 2*k) {                      /* + 2*f(k,m-2k) */
                seq_t t = cur[mi - 2*kk];
                val += t;
                val += t;
            }
            cur[mi] = val;
        }
        /* swap prev and cur for next iteration */
        seq_t *tmp = prev; prev = cur; cur = tmp;
    }

    /* Result is f(n,n) stored in prev[N] after final swap. */
    seq_t result = prev[N];

    /* Free local arrays. */
    free_seq_array(prev);
    free_seq_array(cur);

    return result;
}

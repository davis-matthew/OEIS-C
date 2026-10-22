/*
 * A368343  Square array T(n,k), n >= 3, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} k^(n-j) * floor(j/3).
 * https://oeis.org/A368343
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the square array T(n,k) defined by T(n,k) =
 *            Sum_{j=0..n} floor(j/3) * k^(n-j). The sequence is read by
 *            antidiagonals downwards (i.e., for each d = 0,1,2,... list (n,k)
 *            with n-3 + k = d in increasing row n). Offset: 3. This
 *            implementation directly converts the sequence index to (n,k)
 *            an...
 * Range:     u8 n <= 42, u16 n <= 87, u32 n <= 203, u64 n <= 485,
 *            u128 n <= 1210, big any n
 * Helpers:   _compute_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_compute_T.h"

/* Find the diagonal number d (0‑based) for a given zero‑based index idx:
   smallest d such that (d+1)*(d+2)/2 > idx.  Uses 128‑bit arithmetic
   to avoid overflow.  idx fits in idx_t. */
static idx_t _diagonal_from_index(idx_t idx)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until it satisfies the condition. */
    while ((unsigned __int128)(high + 1) * (high + 2) / 2 <= idx)
        high <<= 1;
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)(mid + 1) * (mid + 2) / 2 > idx)
            high = mid;
        else
            low = mid + 1;
    }
    return high;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;   /* Undefined for offset, return 0. */

    idx_t zero_idx   = n - 3;                /* Zero‑based index in the array. */
    idx_t d          = _diagonal_from_index(zero_idx);
    idx_t prev_terms = d * (d + 1) / 2;      /* Terms before this diagonal. */
    idx_t offset     = zero_idx - prev_terms; /* Position inside diagonal d. */

    int i   = (int)offset;        /* Row offset from the top of the array. */
    int k   = (int)(d - offset);  /* Column index. */
    int n_row = 3 + i;             /* Real row number. */

    return _compute_T(n_row, k);
}

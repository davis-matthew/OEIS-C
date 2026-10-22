/*
 * A368282  Table T(n,k) with n>=1 and k>=0, read by downwards antidiagonals
 *          where the n-th row is the sequence given by a(0) = 0 and a(k) = k
 *          - a(floor(a(k-1)/n)).
 * https://oeis.org/A368282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   ceil_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A368282
 *
 *  Definition:
 *      The infinite table T(n,k) (n≥1, k≥0) is defined recursively by
 *
 *          T(n,0) = 0
 *          T(n,k) = k - T( n , ⌊ T(n,k-1) / n ⌋ )  ,   k≥1
 *
 *      The sequence a(1),a(2),… is read from the table by downwards
 *      antidiagonals:
 *          (1,0), (1,1),(2,0), (1,2),(2,1),(3,0), …
 *
 *  This function implements that definition. The function is independent
 *  of any particular build; it works with arbitrary‐precision seq_t,
 *  uint64_t, and the other integer types supplied by the build.
 *
 *  Complexity: O(k) where k is the column index of the element in its
 *  row; for an n up to 10 000 the maximum k is ≈10 000, well below the
 *  time limits on any build.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ceil_triangular.h"

/* Main function required by the build */
seq_t a(idx_t N)
{
    if (N == 0)           /* offset is 1, but we guard against 0 safely */
        return (seq_t)0;

    /* Determine which anti‑diagonal the index lies on */
    idx_t s = ceil_triangular(N);
    idx_t prev = (idx_t)((__int128)(s - 1) * s / 2);
    idx_t offset = N - prev;                  /* 1 … s */
    idx_t row    = offset;                    /* the parameter n of the row */
    idx_t k      = s - offset;                /* column index (k ≥ 0) */

    /* Allocate space for the row values up to column k */
    seq_t *row_vals = (seq_t *)malloc((k + 1) * sizeof(seq_t));
    if (!row_vals)          /* crash on OOM – acceptable for an online judge */
        abort();

    row_vals[0] = (seq_t)0;
    seq_t prev_val = (seq_t)0;

    for (idx_t i = 1; i <= k; ++i)
    {
        idx_t j = (idx_t)(prev_val / (seq_t)row);   /* floor division */
        seq_t cur = (seq_t)i - row_vals[j];
        row_vals[i] = cur;
        prev_val = cur;
    }

    seq_t result = row_vals[k];
    free(row_vals);
    return result;
}

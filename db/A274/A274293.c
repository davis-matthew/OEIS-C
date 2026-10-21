/*
 * A274293  Triangle read by rows: T(n,k) (0 <= k <= n) given by T(n,0) = 1,
 *          T(n,1) = 2^n - 1, T(n,2) = 2^n - 2, T(n,n-1) = T(n,n) =
 *          binomial(2n-2,n-1); and the other internal entries satisfy T(n,k)
 *          = T(n,k-1) + T(n-1,k).
 * https://oeis.org/A274293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  Triangle rows defined by
   T(n,0) = 1,
   T(n,1) = 2^n - 1,
   T(n,2) = 2^n - 2,
   T(n,n-1) = T(n,n) = binomial(2n-2,n-1),
   and for 3 ≤ k ≤ n-2,
   T(n,k) = T(n,k-1) + T(n-1,k).

   The sequence a(n) is the triangle read by rows. */
#include <stdlib.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/pow2.h"
#include "../utils/binom.h"

/* static cache of computed rows */
static seq_t **_row_cache = NULL;   /* array of pointers to rows */
static idx_t   _cache_size = 0;     /* number of rows currently cached */

/* helper: allocate a new row of length r+1, compute its values */
static void _compute_row(idx_t r)
{
    /* allocate the row */
    seq_t *row = (seq_t *)calloc((size_t)(r + 1), sizeof(seq_t));
    if (!row) abort();  /* safe guard */

    /* base entries */
    row[0] = (seq_t)1;
    if (r >= 1) {
        row[1] = pow2(r) - (seq_t)1;
    }
    if (r >= 2) {
        row[2] = pow2(r) - (seq_t)2;
    }

    /* interior recurrence for 3 ≤ k ≤ r-2 */
    if (r >= 3) {
        seq_t *prev = _row_cache[r - 1];
        for (idx_t k = 3; k <= r - 2; ++k) {
            row[k] = row[k - 1] + prev[k];
        }
    }

    /* boundary values (apply only for r >= 3 to avoid overriding
       the T(n,1) definition for n = 2). */
    if (r >= 3) {
        seq_t last = binom((idx_t)(2 * r - 2), r - 1);
        row[r - 1] = last;
        row[r]     = last;
    }

    /* store the row in the cache */
    _row_cache[r] = row;
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    /* determine row r and position c of a(n) */
    idx_t t = _floor_isqrt_u64(8 * n + 1);
    idx_t r = (t - 1) / 2;
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    /* enlarge cache if necessary */
    if (_cache_size <= r) {
        seq_t **new_cache = (seq_t **)realloc(_row_cache,
                                              (size_t)(r + 1) * sizeof(seq_t *));
        if (!new_cache) abort();
        _row_cache = new_cache;
        for (idx_t i = _cache_size; i <= r; ++i) {
            _compute_row(i);
        }
        _cache_size = r + 1;
    }

    return _row_cache[r][c];
}

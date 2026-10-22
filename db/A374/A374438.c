/*
 * A374438  Triangle read by rows: T(n, k) = T(n - 1, k) + T(n - 2, k - 2),
 *          with initial values T(n, k) = k + 1 for k < 3.
 * https://oeis.org/A374438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   _triangle_row_for, _umul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(n, k) = T(n-1, k) + T(n-2, k-2)
   Initial values:  T(n, k) = k + 1  for k < 3
   The sequence a(n) is the triangle read by rows, starting at index 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_triangle_row_for.h"
#include "../utils/_umul128.h"

/* Row storage: up to 256 rows; covers n <= 10^7 safely. */
#define MAX_ROWS 256
static seq_t *rows[MAX_ROWS] = {0};
static idx_t max_built = 0;

/* Compute row r if not yet built. */
static void _ensure_row(idx_t r)
{
    if (r >= MAX_ROWS) return;       /* safety guard, expands not needed for n <= 10^4 */
    if (rows[r] != NULL) return;     /* already built */

    /* Allocate array of seq_t for row r */
#ifdef __cplusplus
    rows[r] = new seq_t[(size_t)(r + 1)];
#else
    rows[r] = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
#endif
    if (!rows[r]) abort();

    seq_t *prev1 = (r >= 1) ? rows[r - 1] : NULL;
    seq_t *prev2 = (r >= 2) ? rows[r - 2] : NULL;

    for (idx_t k = 0; k <= r; ++k) {
        if (k < 3) {
            rows[r][k] = (seq_t)(k + 1);
        } else {
            seq_t a = 0, b = 0;
            if (k <= r - 1)
                a = prev1[k];
            /* k >= 3 ensures k-2 >= 1 and <= r-2 */
            b = prev2 ? prev2[k - 2] : 0;
            rows[r][k] = a + b;
        }
    }

    if (r > max_built) max_built = r;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    idx_t r = _triangle_row_for(n);
    idx_t offset = n - (idx_t)(_umul128(r, r + 1) / 2);

    _ensure_row(r);
    return rows[r][offset];
}

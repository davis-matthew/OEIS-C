/*
 * A328001  T(n, k) = k!*(n-k)!/(floor(k/2)!*floor((n-k)/2)!)^2. Triangle read
 *          by rows, 0 <= k <= n.
 * https://oeis.org/A328001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = T(i,j) where T(i,j)=j!*(i-j)!/(⌊j/2⌋! * ⌊(i-j)/2⌋!)²
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_isqrt_u64, s_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* row i = floor( (sqrt(8*n+1)-1)/2 ), column j = n - i*(i+1)/2 */

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/s_value.h"

seq_t a(idx_t n)
{
    /* determine row and column of the anti‑diagonal index n */
    idx_t d = 8 * n + 1;                 /* 8*n+1 fits in 64‑bit for the test range */
    idx_t r = floor_isqrt_u64(d);        /* floor(sqrt(8*n+1)) */
    idx_t row = (r - 1) / 2;              /* row index i */
    idx_t offset = (idx_t)((seq_t)row * (row + 1) / 2); /* first index of this row */
    idx_t k = n - offset;                 /* column index j */

    /* T(i,j) = s(j) * s(i-j) */
    seq_t s1 = s_value(k);
    seq_t s2 = s_value(row - k);
    return s1 * s2;
}

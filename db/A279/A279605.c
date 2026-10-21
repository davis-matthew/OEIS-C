/*
 * A279605  Triangle T(n, k) read by rows: minimal number of knight moves to
 *          reach the central square on a (2*n+1) X (2*n+1) board starting
 *          from the k-th outermost square counted from middle of first rank
 *          for k = 1..n+1, or -1 if reaching the central square is
 *          impossible.
 * https://oeis.org/A279605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, tabl
 * Formula:   A279605 uses infinite‑board knight distance for boards of size
 *            2*n+1 with special handling for n = 0 and n = 1 (both entries of
 *            row 1 are -1)
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   knight_dist_infinite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/knight_dist_infinite.h"

static idx_t int_sqrt_u64(idx_t n)
{
    /* Integer square root via binary search. */
    idx_t lo = 0, hi = 65536;               /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid != 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row r and position k such that
       r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    if (n > 0) {
        idx_t t = int_sqrt_u64(8llu * n + 1);
        r = (t - 1) / 2;
        /* Correct possible over‑estimate. */
        if ((r + 1) * (r + 2) / 2 <= n) r++;
        while (r * (r + 1) / 2 > n) r--;
    }

    /* Special case handling. */
    if (r == 0) return 0;          /* the single 0 of row 0 */
    if (r == 1) return (seq_t)-1;   /* both entries of row 1 are -1 */

    /* Compute k (1‑based) and column index (0‑based). */
    idx_t start = r * (r + 1) / 2;          /* first index of this row */
    idx_t k = n - start + 1;                /* 1‑based position in row */
    idx_t col = k - 1;                      /* left‑half columns: 0 .. r */

    /* Coordinates of the starting square: (col, 0).
       Center is at (r, r). */
    seq_t dx = (seq_t)(r >= col ? r - col : col - r);
    seq_t dy = (seq_t)r;

    return knight_dist_infinite(dx, dy);
}

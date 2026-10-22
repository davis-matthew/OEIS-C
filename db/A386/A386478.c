/*
 * A386478  Array read by upward antidiagonals: T(k,n) = 1 (k = 0, n >= 0),
 *          T(k,n) = k^2*n^2/2 - (3*k-4)*n/2 + 1 (k >= 1, n >= 0).
 * https://oeis.org/A386478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(k, ncol) where T(0, *) = 1 T(k, n) = k²·n²/2 -
 *            (3·k-4)·n/2 + 1 (k ≥ 1, n ≥ 0) The array is read by upward
 *            antidiagonals: Let d be the largest integer with d·(d+1)/2 ≤ n.
 *            Let pos = n - d·(d+1)/2 (0 ≤ pos ≤ d). Then k = d - pos ncol =
 *            pos
 * Range:     u8 n <= 47, u16 n <= 794, u32 n <= 9754, u64 n <= 9754,
 *            u128 n <= 9754, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binary search for largest d with d*(d+1)/2 <= n */
static idx_t _diag_of(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;                /* d */
}

seq_t a(idx_t n)
{
    idx_t d   = _diag_of(n);
    idx_t before = d * (d + 1) / 2;
    idx_t pos = n - before;            /* position inside diagonal */
    idx_t k   = d - pos;               /* row index (0‑based) */
    idx_t ncol = pos;                 /* column index (0‑based) */

    if (k == 0)                       /* T(0, *) = 1 */
        return (seq_t)1;

    seq_t kk = (seq_t)k;
    seq_t cc = (seq_t)ncol;

    /* numerator = k²·n² - 3·k·n + 4·n  (avoid underflow) */
    seq_t term1 = kk * kk * cc * cc;   /* k²·n²   */
    seq_t term3 = 4 * cc;              /* 4·n     */
    seq_t term2 = 3 * kk * cc;         /* 3·k·n   */
    seq_t numerator = term1 + term3 - term2;

    return numerator / 2 + 1;          /* result = numerator/2 + 1 */
}

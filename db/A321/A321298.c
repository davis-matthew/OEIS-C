/*
 * A321298  Triangle read by rows: T(n,k) is the number of the k-th eliminated
 *          person in the Josephus elimination process for n people and a
 *          count of 2, 1 <= k <= n.
 * https://oeis.org/A321298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   A321298 via recurrence: * T(n,k) = 2*k if k <= n/2; * T(2*n,k) =
 *            2*T(n,k-n) - 1 if k > n; * T(2*n+1,k) = 2*T(n,k-n-1) + 1 if k >
 *            n+1; * T(2*n+1,n+1) = 1. * a(n) is the flattened triangle read
 *            row-wise
 * Range:     u8 n <= 11325, u16 n <= 11325, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 114 terms through n = 9825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive computation of T(n,k) */
static inline seq_t T_rec(idx_t n, idx_t k)
{
    if (k <= n/2) {                     /* 2*k part */
        return (seq_t)(2*k);
    }
    if ((n & 1) == 0) {                 /* even case: n = 2*m */
        return 2 * T_rec(n/2, k - n/2) - 1;
    }
    /* odd case: n = 2*m+1 */
    idx_t m = n/2;                      /* floor((n-1)/2) */
    if (k == m + 1)                     /* special middle element */
        return (seq_t)1;
    return 2 * T_rec(m, k - m - 1) + 1;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                      /* row number (1‑based) */
    __int128 prev = (__int128)(r-1) * r / 2;
    idx_t c = n - (idx_t)prev;          /* column number (1‑based) */

    return T_rec(r, c);
}

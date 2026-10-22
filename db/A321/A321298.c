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
 * Range:     u8 n <= 9825, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   T_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/T_rec.h"

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

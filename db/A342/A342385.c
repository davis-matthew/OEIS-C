/*
 * A342385  Triangle T(n,k) read by rows (n >= 0, 0 <= k <= n): first column
 *          is A001477 and column k > 0 is k*A028310.
 * https://oeis.org/A342385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique non‑negative integer with r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, and let k = n - r(r+1)/2. Then a(n) is the T(r,k)
 *            entry of the triangle defined by T(r,0) = T(r,r) = r, T(r,k) =
 *            k*(r-k) for 0 < k < r. This yields the sequence
 *            0,1,1,2,1,2,3,2,2,3,4,3,4,3,4,5,4,6,6,4,5,6,5,8,
 * Range:     u8 n <= 543, u16 n <= 9716, u32 n <= 9716, u64 n <= 9716,
 *            u128 n <= 9716, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2
       Binary search on r in the range [0, n] (n fits in idx_t).   */
    idx_t low = 0;
    idx_t high = n;          /* inclusive upper bound for r */

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;       /* upper mid */
        /* Use 128‑bit arithmetic to avoid overflow on r(r+1)/2.   */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r = low;
    /* Compute offset k = n - r(r+1)/2 */
    idx_t prefix = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - prefix;

    if (k == 0 || k == r)
        return (seq_t)r;                 /* T(r,0) = T(r,r) = r       */
    else
        return (seq_t)k * (seq_t)(r - k); /* T(r,k) = k * (r-k)         */
}

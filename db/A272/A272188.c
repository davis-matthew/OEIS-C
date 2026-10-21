/*
 * A272188  Triangle with 2*n+1 terms per row, read by rows: the first row is
 *          1 (by decree), following rows contain 0 to 2n+1 but omitting 2n.
 * https://oeis.org/A272188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(0)=1, for n>0 let r=floor(sqrt(n)); let t=n-r^2. Then a(n)=t
 *            if t<2*r, a(n)=t+1 if t==2*r. (t runs from 0 to 2*r) This
 *            implements the “triangle with 2n+1 terms per row” described
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t, returns floor(sqrt(n)). */
static inline idx_t isqrt64(idx_t n)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;   /* sqrt(UINT64_MAX) < 2^32 */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t r = isqrt64(n);          /* r = floor(sqrt(n))  */
    idx_t t = n - r * r;           /* offset within row */

    /* Row 0 is handled above. For r>0, row length = 2r+1,
       numbers ascending from 0..2r-1, then 2r+1. */
    if (t < 2 * r)
        return (seq_t)t;
    else /* t == 2*r */
        return (seq_t)(t + 1);
}

/*
 * A374708  Triangle T read by rows: T(n,k) = (n - k)*n*(4*n^2 - 4*n*k + 2*k^2
 *          - 1 + (-1)^k)/4, with 0 <= k < n.
 * https://oeis.org/A374708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A374708
 * Triangle T read by rows: T(n,k) = (n-k)*n*(4*n^2 - 4*n*k + 2*k^2 - 1 + (-1)^k)/4
 * for 0 <= k < n, indexed by rows.  a(n) lists the entries row‑by‑row,
 * starting with a(1)=T(1,0).
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the row r such that (r-1)*r/2 < n <= r*(r+1)/2. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row index (1‑based)          */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_tri - 1; /* column index (0‑based)       */

    /* Compute T(r,k) using the integer formula. */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    seq_t s  = rn - kn;                                   /* (r-k)              */
    seq_t base = 4 * rn * rn - 4 * rn * kn + 2 * kn * kn - 1;
    if (k & 1) /* k odd -> subtract 1 */ base -= 1;
    else       /* k even -> add 1    */ base += 1;        /* 4r^2 – 4rk + 2k^2 –1 + (-1)^k */

    seq_t prod = s * rn * base;                           /* numerator         */
    seq_t res  = prod / 4;                                 /* exact division    */
    return res;
}

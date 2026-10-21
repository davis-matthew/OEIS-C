/*
 * A294317  Triangle read by rows: T(n, k) = 2*n-k, k <= n.
 * https://oeis.org/A294317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2*i - j where i = ⌊(√(8n+1) - 1)/2⌋ j = n - i(i+1)/2 This
 *            is the natural numbering of the triangle T(n,k)=2n-k
 * Range:     u8 n <= 8255, u16 n <= 8558, u32 n <= 8558, u64 n <= 8558,
 *            u128 n <= 8558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned value */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t lo = 1, hi = (n >> 1) + 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > n / mid) {
            hi = mid - 1;
        } else {
            /* mid*mid <= n ; check for exact equality */
            if (mid * mid == n) return mid;
            lo = mid + 1;
        }
    }
    return hi;           /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* compute i = floor((sqrt(8n+1)-1)/2) */
    idx_t d = (idx_t)8 * n + 1;
    idx_t sqrt_d = isqrt_u64(d);
    idx_t i = (sqrt_d - 1) / 2;

    /* prefix = i(i+1)/2, the number of terms before row i */
    idx_t prefix = i * (i + 1) / 2;
    idx_t j = n - prefix;    /* column index within the row, 0 ≤ j ≤ i */

    /* compute T(i,j) = 2*i - j, converted to seq_t */
    seq_t val = (seq_t)2 * (seq_t)i - (seq_t)j;
    return val;
}

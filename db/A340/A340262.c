/*
 * A340262  T(n, k) = multinomial(n + k/2; n, k/2) if k is even else 0.
 *          Triangle read by rows, for 0 <= k <= n.
 * https://oeis.org/A340262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = multinomial(n + k/2; n, k/2) if k is even, else 0
 *            (filling the triangle by rows: 0 ≤ k ≤ n)
 * Range:     u8 n <= 33, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Integer square root: floor(sqrt(x)) for unsigned 64‑bit integers.  */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {          /* avoid division by zero */
            lo = 1;
            continue;
        }
        if (mid > x / mid)       /* mid*mid > x but avoid overflow */
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Find row r and column k for index n.  */
    idx_t r = (isqrt_u64((n << 3) + 1) - 1) / 2;          /* r = floor((sqrt(8n+1)-1)/2) */
    idx_t start = r * (r + 1) / 2;                        /* first index of row r */
    idx_t k = n - start;                                  /* column index */

    /* Even columns give the multinomial value, odd columns give 0. */
    if (k & 1U)
        return (seq_t)0;

    /* t = k/2; compute binomial(r + t, t) i.e. multinomial(r + t; r, t). */
    idx_t t = k >> 1;
    return binomial(r + t, t);
}

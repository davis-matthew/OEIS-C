/*
 * A370915  A(n, k) = 4^n*Pochhammer(k/4, n). Square array read by ascending
 *          antidiagonals.
 * https://oeis.org/A370915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A(i,j) where A(i,j) = ∏_{t=0}^{i-1} (4*t + j) // product
 *            of the row/column indices The square array is read by
 *            antidiagonals. i and j are obtained from the index n by s =
 *            floor((sqrt(8*n+1)-1)/2) cum = s*(s+1)/2 // terms before
 *            diagonal s offset = n - cum i = s - offset j = offset
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root (floor) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t M   = 8 * nn + 1;
    uint64_t t   = isqrt_u64(M);
    uint64_t s   = (t - 1) / 2;                 /* diagonal index   */
    uint64_t cum = s * (s + 1) / 2;             /* terms before diag */

    uint64_t offset = nn - cum;                 /* position inside diag   */
    uint64_t i       = s - offset;              /* row index              */
    uint64_t j       = offset;                  /* column index           */

    if (i == 0) return (seq_t)1;

    seq_t prod = 1;
    for (uint64_t k = 0; k < i; ++k) {
        seq_t term = (seq_t)(4 * k + j);
        prod *= term;
    }
    return prod;
}

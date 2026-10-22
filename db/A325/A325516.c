/*
 * A325516  Triangle read by rows: T(n, k) = (1/4)*n*(1 - (-1)^(n - k) + 2*(n
 *          - k)^2), with 0 <= k < n.
 * https://oeis.org/A325516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th element of the triangle T(m,k) = m * ((m-k)&1 +
 *            (m-k)^2) / 2 read row‑wise in order. It is obtained by 1.
 *            Finding the row m such that m(m+1)/2 ≥ n 2. Computing the column
 *            k = n - (m-1)m/2 - 1 3. Evaluating T(m,k)
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned values. */
static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (1ULL << 32);          /* sqrt(2^64) = 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Assume n >= 1.  Determine the row m. */
    idx_t disc = 8 * n + 1;                /* discriminant */
    idx_t sqrt_d = floor_isqrt_u64(disc);
    idx_t m = (sqrt_d - 1) >> 1;           /* floor of the inverse */
    if ((unsigned __int128)m * (m + 1) / 2 < n)
        m++;

    /* Column k within that row (0‑based). */
    idx_t prev = (m - 1) * m / 2;
    idx_t k = n - prev - 1;

    /* Compute T(m,k).  Use the parity trick. */
    idx_t d = m - k;                       /* d = m - k */
    seq_t odd = d & 1;                     /* 0 or 1 */
    seq_t d_sq = (seq_t)d * (seq_t)d;      /* (m-k)^2 */
    seq_t value = odd + d_sq;              /* odd + d^2 */
    seq_t result = (seq_t)m * value / 2;   /* m * (odd + d^2) / 2 */

    return result;
}

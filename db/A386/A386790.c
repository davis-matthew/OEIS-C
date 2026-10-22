/*
 * A386790  Triangle read by rows: T(n, k) = n/k if k divides n else 0 for k >
 *          0, T(n, 0) = 0^n.
 * https://oeis.org/A386790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) is obtained by reading the triangle * T(r,k) =
 *            (k==0)?(r==0?1:0):((r%k==0)?(r/k):0) * row r is the unique
 *            integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2 * and column k = n -
 *            r*(r+1)/2. * This matches the specification of OEIS A386790
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x < 2^64. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 1;
    /* Find an upper bound for the square root. */
    while ((__int128)hi * hi <= x) hi <<= 1;
    /* Binary search. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Compute row r via triangular numbers. */
    idx_t t = _isqrt_u64(8 * n + 1);    /* floor(sqrt(8*n+1)) */
    idx_t r = (t - 1) / 2;              /* row index */
    idx_t prev = r * (r + 1) / 2;       /* elements before row r */
    idx_t k = n - prev;                 /* column index in the row (0 <= k <= r) */

    if (k == 0) {
        /* T(r,0) = 0^r : 1 when r==0, else 0. */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* For k > 0: return r/k if k divides r, otherwise 0. */
    if (r % k == 0)
        return (seq_t)(r / k);
    else
        return (seq_t)0;
}

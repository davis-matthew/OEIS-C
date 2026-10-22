/*
 * A324224  Total number T(n,k) of 1's in falling diagonals with index k in
 *          all n X n permutation matrices divided by |k|!; triangle T(n,k),
 *          n>=1, 1-n<=k<=n-1, read by rows.
 * https://oeis.org/A324224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 70, u32 n <= 155, u64 n <= 418,
 *            u128 n <= 1186, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (N - t) * (N-1)! / t!
 * where N = ceil(sqrt(n)), t = |(n - (N-1)^2) - 1 - (N-1)|.
 * This yields the triangle entry T(N,k) for k = (n-(N-1)^2)-1-(N-1).
 * The triangle is defined by
 *   T(N,k) = (N - |k|) * (N-1)! / |k|!
 * for |k| < N, otherwise 0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- helper: floor sqrt for uint64_t ---------- */
static inline idx_t _floor_sqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) */
    idx_t low = 0, high = (n >> 1) + 1; /* high <= sqrt(n)+1 */
    if (n < 2) return n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            if (mid > (n / mid)) { /* overflow guard */
                low = mid;
            } else {
                low = mid + 1;
            }
        } else {
            high = mid - 1;
        }
    }
    return high; /* high is floor sqrt */
}

static inline idx_t _ceil_sqrt_u64(idx_t n)
{
    idx_t r = _floor_sqrt_u64(n);
    return (r * r == n) ? r : r + 1;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    idx_t N = _ceil_sqrt_u64(n);           /* row index */
    idx_t prev_sq = (N - 1) * (N - 1);     /* (N-1)^2  */
    idx_t offset = n - prev_sq;            /* 1 .. 2N-1 */
    idx_t j0 = offset - 1;                 /* 0 .. 2N-2  */
    idx_t t = (j0 >= (N - 1)) ? (j0 - (N - 1)) : ((N - 1) - j0); /* |k| */

    /* compute (N-t) * (N-1)! / t! */
    seq_t prod = 1;
    for (idx_t i = t + 1; i <= N - 1; ++i) {
        prod *= (seq_t)i;
    }
    seq_t result = (seq_t)(N - t) * prod;
    return result;
}

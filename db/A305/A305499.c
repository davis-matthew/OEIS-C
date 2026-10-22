/*
 * A305499  Square array A(n,k), n > 0 and k > 0, read by antidiagonals, with
 *          initial values A(1,k) = k and recurrence equations A(n+1,k) =
 *          A(n,k) for 0 < k <= n and A(n+1,k) = A(n,k) - A000035(n+k) for 0 <
 *          n < k.
 * https://oeis.org/A305499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A(nrow,kcol) where k = smallest integer with k*(k+1)/2 ≥
 *            n prev = (k-1)*k/2 offset = n - prev - 1 nrow = k - offset kcol
 *            = offset + 1 A(n,k) = floor((k+1)/2) if k ≤ n = floor((k+1)/2) +
 *            floor((k+1-n)/2) if k > n (indices are 1‑based)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build. */

seq_t a(idx_t n)
{
    /* find k: minimal integer with k*(k+1)/2 ≥ n */
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 < n) {
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    idx_t k = hi;                     /* k satisfies k*(k+1)/2 ≥ n */

    /* previous cumulative count */
    unsigned __int128 prev128 = (unsigned __int128)(k - 1) * k / 2;
    idx_t prev = (idx_t)prev128;

    idx_t offset = n - prev - 1;

    idx_t nrow = k - offset;
    idx_t kcol = offset + 1;

    /* compute A(nrow, kcol) */
    if (kcol <= nrow) {
        return ((seq_t)kcol + 1) / 2;
    } else {
        seq_t term1 = ((seq_t)kcol + 1) / 2;
        seq_t term2 = (((seq_t)kcol + 1 - (seq_t)nrow)) / 2;
        return term1 + term2;
    }
}

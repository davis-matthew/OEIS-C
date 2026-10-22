/*
 * A394016  Triangle read by rows: T(n, k) = 1 if k appears in the n X n
 *          multiplication table, otherwise 0.
 * https://oeis.org/A394016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9042, u16 n <= 9042, u32 n <= 9042, u64 n <= 9042,
 *            u128 n <= 9042, big any n
 * Helpers:   cumSum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394016:
   a(n) is the n-th entry of the concatenation
   of the tables 1×1, 2×2, …, r×r.
   Let r be the unique integer with
        sum_{i=1}^{r-1} i^2  <  n  ≤  sum_{i=1}^{r} i^2 .
   Let k = n - sum_{i=1}^{r-1} i^2  (1 ≤ k ≤ r²).
   Then a(n) = 1 iff k admits a factorization a·b with
   1 ≤ a,b ≤ r, otherwise a(n) = 0.   */
#include <stdint.h>
#include "../utils/cumSum.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n */
    idx_t low = 1, high = 1;
    while (cumSum(high) < n) {
        if (high > (UINT64_MAX >> 1)) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (cumSum(mid) >= n)
            high = mid - 1;
        else
            low = mid + 1;
    }
    idx_t r = low;                      // minimal r with cumSum(r) ≥ n
    idx_t prevSum = (r > 1) ? cumSum(r - 1) : 0;
    idx_t k = n - prevSum;              // 1‑based position inside row r

    /* k is always ≤ r².  Check whether it can be written as a·b with
       1 ≤ a,b ≤ r.  If k ≤ r then a=1,b=k works immediately. */
    if (k <= r)
        return (seq_t)1;

    /* Search for any divisor d of k with d ≤ r and k/d ≤ r. */
    for (idx_t d = 2; d * d <= k; ++d) {
        if (k % d == 0) {
            idx_t e = k / d;
            if (e <= r)
                return (seq_t)1;
        }
    }
    return (seq_t)0;
}

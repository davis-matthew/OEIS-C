/*
 * A258579  Triangle read by rows: T(n,k) = number of partial idempotent
 *          mappings (of an n-chain) with (right) waist exactly k.
 * https://oeis.org/A258579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = Σ_{m=1..k}
 *            binomial(k-1,m-1)*(m+1)^(r-m) with r = floor( solution of
 *            r(r+1)/2 ≤ n ), k = n - r(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t lhs = (__uint128_t)mid * (mid + 1);
        __uint128_t rhs = (__uint128_t)2 * n;
        if (lhs <= rhs) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Columns preceding this row: prev_total = r(r+1)/2. */
    idx_t prev_total = ((__uint128_t)r * (r + 1)) >> 1;
    idx_t k = n - prev_total;

    if (k == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t m = 1; m <= k; ++m) {
        seq_t bin = binomial(k - 1, m - 1);
        seq_t pw  = big_pow((seq_t)(m + 1), (seq_t)(r - m));
        sum += bin * pw;
    }
    return sum;
}

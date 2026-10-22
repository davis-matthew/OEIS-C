/*
 * A350771  Triangle read by rows: T(n, k) = (2^(n-k-1) + 2^k -
 *          2)*binomial(n-1, k), 0 <= k <= n-1.
 * https://oeis.org/A350771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = smallest integer such that r*(r+1)/2 ≥ n,
 *            k = n - r*(r-1)/2 - 1, and T(r,k) = (2^(r-k-1)+2^k-2) *
 *            binomial(r-1,k)
 * Range:     u8 n <= 23, u16 n <= 80, u32 n <= 259, u64 n <= 957,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // Find the row r containing the n-th element of the triangle
    idx_t r = 1;
    for (;; ++r) {
        __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
        if (tri >= n)
            break;
    }

    __uint128_t tri_before = (__uint128_t)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - tri_before - 1);          // column in the row

    seq_t pow_left  = pow2(r - k - 1);              // 2^(r-k-1)
    seq_t pow_right = pow2(k);                      // 2^k
    seq_t mult      = pow_left + pow_right - (seq_t)2;  // (2^(r-k-1)+2^k-2)

    seq_t bin = binomial(r - 1, k);                 // binomial(r-1,k)

    return mult * bin;                              // T(r,k)
}

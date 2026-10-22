/*
 * A351339  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) is Sum_{j=0..n} k^j * j^(n-j).
 * https://oeis.org/A351339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Sum_{j=0..t} (k^j) * (j^(t-j)), where t and k are
 *            obtained from n by let d satisfy d*(d+1)/2 <= n < (d+1)*(d+2)/2,
 *            then t = n - d*(d+1)/2, k = d - t. The term a(n) is the element
 *            T(t,k) of the square array
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Find diagonal index d using binary search on d*(d+1)/2 <= n */
    uint64_t low = 0, high = ((uint64_t)1 << 33) - 1;   /* enough for n up to 2^64-1 */
    uint64_t d = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            d = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Compute row (t) and column (k) indices */
    __uint128_t start = (__uint128_t)d * (d + 1) / 2;
    idx_t t = n - (idx_t)start;          /* t ranges 0 .. d */
    idx_t row = t;                       /* n' = t */
    idx_t col = d - t;                   /* k' = d - t */

    /* Evaluate the sum */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= row; ++j) {
        seq_t term = pow_seq((seq_t)col, (int)j);            /* col^j */
        term *= pow_seq((seq_t)j, (int)(row - j));          /* j^(row-j) */
        sum += term;
    }
    return sum;
}

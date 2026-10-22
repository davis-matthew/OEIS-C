/*
 * A391010  Triangle read by rows: T(n, k) is the maximum number of divisors a
 *          number with k distinct and n total prime factors can have.
 * https://oeis.org/A391010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten triangle T(n,k) where T(n,k) = (q+1)^r * q^(k-r)
 *            with m = n + k, q = floor(m/k), r = m mod k. For k = 0, a(0) = 1
 *            else a(n,k) = 0
 * Range:     u8 n <= 43, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Determine row index r and column index k (k distinct primes). */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t   = mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Special case: n == 0 => a(0) = 1 */
    if (r == 0) return (seq_t)1;

    /* Column index k = position within the row */
    idx_t k = n - r * (r + 1) / 2;
    if (k == 0) return (seq_t)0;   /* T(r,0) = 0 for r > 0 */

    /* Compute m, q, remainder */
    idx_t m   = r + k;
    idx_t q   = m / k;
    idx_t rem = m % k;

    seq_t base1 = (seq_t)(q + 1);
    seq_t base2 = (seq_t)q;
    seq_t pow1  = big_pow(base1, (seq_t)rem);
    seq_t pow2  = big_pow(base2, (seq_t)(k - rem));

    return pow1 * pow2;
}

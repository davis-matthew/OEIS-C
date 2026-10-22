/*
 * A345096  a(n) = Sum_{k=1..n} Sum_{i=1..k} k^(1 - ceiling(n/k^i) +
 *          floor(n/k^i)).
 * https://oeis.org/A345096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{i=1..k} k^(1 - ceil(n/k^i) +
 *            floor(n/k^i)) The exponent simplifies to 1 if k^i divides n,
 *            otherwise 0, so a(n) = Sum_{k=1..n} [ d_k*(k-1) + k ] where d_k
 *            = min(v_k, k) and v_k is the largest e with k^e | n
 * Range:     u8 n <= 20, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t d;
        if (k == 1) {
            /* k = 1 divides n for all i; d_1 = 1 */
            d = 1;
        } else {
            idx_t m = n;
            idx_t e = 0;
            while (m % k == 0) {
                m /= k;
                ++e;
            }
            d = (e > k ? k : e);
        }
        sum += (seq_t)d * (seq_t)(k - 1) + (seq_t)k;
    }
    return sum;
}

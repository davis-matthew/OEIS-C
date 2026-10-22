/*
 * A362348  a(n) = n! * Sum_{k=0..floor(n/3)} k^k / (k! * (n-3*k)!).
 * https://oeis.org/A362348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^k / (k! * (n-3*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t sum = 1;           /* contribution of k = 0 : 0^0 = 1 */
    seq_t term = 1;          /* t_0 = 1 = n!/(n! * 0!)   */
    idx_t maxk = n / 3;      /* upper bound for k       */
    seq_t N = (seq_t)n;      /* promote n to seq_t      */

    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t m = k - 1;                     /* previous index      */
        seq_t a_val = N - (seq_t)(3 * m);    /* n - 3*(k-1)         */

        /* prod = (n - 3*(k-1)) * (n - 3*(k-1) - 1) * (n - 3*(k-1) - 2) */
        seq_t prod = a_val * (a_val - 1);
        prod *= (a_val - 2);

        /* t_k = t_{k-1} * prod / k  (exact division) */
        term = term * prod / (seq_t)k;

        /* pk = k^k  (defined as 1 for k = 0, handled above) */
        seq_t pk = pow_uint((uint64_t)k, k);

        sum += term * pk;
    }

    return sum;
}

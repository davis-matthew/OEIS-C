/*
 * A328337  The number whose binary indices are the nontrivial divisors of n
 *          (greater than 1 and less than n).
 * https://oeis.org/A328337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, 1<d<n} 2^(d-1)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            /* add contribution of divisor d */
            res += pow2(d - 1);
            /* add contribution of complementary divisor n/d if distinct */
            idx_t e = n / d;
            if (e != d) {
                res += pow2(e - 1);
            }
        }
    }
    return res;
}

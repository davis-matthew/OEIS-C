/*
 * A271654  a(n) = Sum_{k|n} binomial(n-1,k-1).
 * https://oeis.org/A271654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k|n} binomial(n-1,k-1)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   mult_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mult_binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds, but safe */

    seq_t sum = 0;
    idx_t n_minus1 = n - 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += mult_binom(n_minus1, d - 1);
            idx_t e = n / d;
            if (e != d) {
                sum += mult_binom(n_minus1, e - 1);
            }
        }
    }
    return sum;
}

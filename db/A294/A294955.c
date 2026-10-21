/*
 * A294955  a(n) = Sum_{d|n} d^(2*n+2).
 * https://oeis.org/A294955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(2*n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t exp = (seq_t)(n) * 2 + 2;
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            seq_t term1 = big_pow((seq_t)d1, exp);
            sum += term1;
            if (d1 != d2) {
                seq_t term2 = big_pow((seq_t)d2, exp);
                sum += term2;
            }
        }
    }
    return sum;
}

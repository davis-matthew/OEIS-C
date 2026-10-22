/*
 * A308753  a(n) = Sum_{d|n} d^(2*(d-1)).
 * https://oeis.org/A308753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(2*(d-1))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 */
    seq_t result = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t term = big_pow((seq_t)d, (seq_t)(2 * (d - 1)));
            result += term;
            idx_t e = n / d;
            if (e != d) {
                seq_t term2 = big_pow((seq_t)e, (seq_t)(2 * (e - 1)));
                result += term2;
            }
        }
    }
    return result;
}

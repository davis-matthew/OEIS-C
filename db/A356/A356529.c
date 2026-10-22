/*
 * A356529  a(n) = (n-1)! * Sum_{d|n} d^(n-d+1).
 * https://oeis.org/A356529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n} d^{(n-d+1)}
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* offset starts at 1 */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t exp = (seq_t)(n - d + 1);
            sum += big_pow((seq_t)d, exp);
        }
    }

    seq_t fact = factorial(n - 1);
    return fact * sum;
}

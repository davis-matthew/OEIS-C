/*
 * A245714  Least number k > 0 such that n + k! is prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A245714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that n + k! is prime, or 0 if none
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1) return 1;

    /* Even n: only k = 1 can work, because k! is even for k >= 2 */
    if ((n & 1) == 0) {
        seq_t val = (seq_t)n + (seq_t)1;
        return is_prime_big(val) ? 1 : 0;
    }

    /* Odd n: skip k = 1 (sum is even > 2), start from k = 2 */
    seq_t fact = 1;
    for (idx_t k = 2; k <= n; ++k) {
        fact *= (seq_t)k;                          /* compute k! */
        seq_t val = (seq_t)n + fact;               /* n + k!   */
        if (is_prime_big(val))                     /* primality test */
            return (seq_t)k;                       /* found minimal k */
    }
    return 0;                                      /* no such k */
}

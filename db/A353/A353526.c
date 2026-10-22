/*
 * A353526  The smallest prime not dividing n, reduced modulo 4.
 * https://oeis.org/A353526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (smallest prime not dividing n) mod 4
 * Range:     u8 n <= 9232, u16 n <= 9232, u32 n <= 9232, u64 n <= 9232,
 *            u128 n <= 9232, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    for (idx_t p = 2; ; ++p) {
        if (!is_prime(p))
            continue;   /* skip composites */
        if (n % p != 0)
            return (seq_t)(p % 4);
    }
}

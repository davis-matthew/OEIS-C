/*
 * A250480  a(1) = 0, and for n > 1: if n is a prime, a(n) = n, otherwise a(n)
 *          = A020639(n) - 1, where A020639(n) gives the least prime dividing
 *          n.
 * https://oeis.org/A250480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is prime; otherwise a(n) = A020639(n) - 1, where
 *            A020639(n) is the least prime dividing n. a(1) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    uint64_t spf = _smallest_prime_factor((uint64_t)n);

    if (spf == (uint64_t)n)          /* n is prime */
        return (seq_t)spf;
    else                              /* n is composite */
        return (seq_t)spf - 1;
}

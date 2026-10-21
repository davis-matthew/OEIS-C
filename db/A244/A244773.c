/*
 * A244773  Prime numbers ending in the prime number 67.
 * https://oeis.org/A244773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 127, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes p such that p % 100 == 67 (p = 100·k + 67) */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset handling not required */

    idx_t  count = 0;
    seq_t  cand  = 67;          /* first number of the form 100·k + 67 */

    for (;;) {
        if (is_prime_big(cand)) {
            count++;
            if (count == n)
                return cand;
        }
        cand += 100;           /* next candidate number */
    }
}

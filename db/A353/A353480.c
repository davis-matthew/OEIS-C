/*
 * A353480  a(n) = 1 if n is an odd semiprime, otherwise 0.
 * https://oeis.org/A353480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is an odd semiprime (product of two odd primes,
 *            counted with multiplicity), otherwise 0
 * Range:     u8 n <= 9550, u16 n <= 9550, u32 n <= 9550, u64 n <= 9550,
 *            u128 n <= 9550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2, there are no semiprimes. */
    if (n < 2)
        return 0;

    /* An odd semiprime must be odd. */
    if ((n & 1) == 0)
        return 0;

    /* Work with a 64‑bit value; n fits in uint64_t because idx_t is uint64_t. */
    uint64_t x = (uint64_t)n;
    int count = 0;

    /* Check odd divisors from 3 upwards. */
    for (uint64_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) {
            ++count;
            if (count > 2)
                return 0;
            x /= d;
        }
    }

    /* If any prime factor remains (x > 1) it contributes one more factor. */
    if (x > 1)
        ++count;

    return (seq_t)(count == 2);
}

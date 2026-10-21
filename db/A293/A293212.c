/*
 * A293212  Binary XOR of the distinct prime divisors of n.
 * https://oeis.org/A293212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = XOR of distinct prime divisors of n
 * Range:     u8 n <= 256, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) = n if n is prime) */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

/* Compute the XOR of the distinct prime factors of n.
   The function works for any n >= 2; for n < 2 it returns 0
   (the offset of the OEIS entry starts at 2). */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Work with a 64‑bit copy to use the helper. */
    uint64_t m = (uint64_t)n;
    seq_t res = 0;

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        res ^= (seq_t)p;          /* XOR the distinct prime divisor.   */
        while (m % p == 0)
            m /= p;              /* Skip repeated factors.           */
    }
    return res;
}

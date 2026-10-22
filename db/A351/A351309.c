/*
 * A351309  Sum of the 4th powers of the square divisors of n.
 * https://oeis.org/A351309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2|n} (d^2)^4 = Sum_{k^2|n} k^8 Let a(n) be the
 *            multiplicative function with a(p^e) = (p^{8*(1+floor(e/2))} -
 *            1)/(p^8 - 1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 24, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   part_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/part_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard anyway */
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t nn = (uint64_t)n;   /* local mutable copy for trial division */

    /* Handle factor 2 separately. */
    idx_t exp = 0;
    while ((nn & 1ULL) == 0) {
        ++exp;
        nn >>= 1;
    }
    if (exp > 0) {
        result *= part_prime((seq_t)2, exp);
    }

    /* Trial division by odd numbers up to sqrt(nn). */
    for (idx_t p = 3; p * p <= nn; p += 2) {
        if (nn % p != 0) continue;
        exp = 0;
        while (nn % p == 0) {
            ++exp;
            nn /= p;
        }
        result *= part_prime((seq_t)p, exp);
    }

    /* If remaining nn > 1 it is a prime factor of exponent 1. */
    if (nn > 1) {
        /* For exponent 1 the contribution is 1, so nothing to multiply. */
        result *= part_prime((seq_t)nn, 1);
    }

    return result;
}

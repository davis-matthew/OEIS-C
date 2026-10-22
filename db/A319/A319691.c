/*
 * A319691  a(n) = 1 if n is either 1 or divisible only by primes congruent to
 *          1 mod 3, 0 otherwise.
 * https://oeis.org/A319691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if n = 1 or all prime factors of n are congruent to 1
 *            mod 3, 0 otherwise
 * Range:     u8 n <= 9940, u16 n <= 9940, u32 n <= 9940, u64 n <= 9940,
 *            u128 n <= 9940, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The sequence is multiplicative with a(p) = 1 if p ≡ 1 (mod 3), otherwise 0. */
seq_t a(idx_t n)
{
    if (n == 1)               /* base case: a(1) = 1 */
        return 1;

    uint64_t m = (uint64_t)n;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        if (2 % 3 != 1)       /* 2 ≡ 2 (mod 3) → reject */
            return 0;
        while ((m & 1ULL) == 0)
            m >>= 1;          /* divide out all 2's */
    }

    /* trial divide for odd potential factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (p % 3 != 1)   /* prime factor not ≡ 1 (mod 3) */
                return 0;
            while (m % p == 0)
                m /= p;       /* remove this prime completely */
        }
    }

    /* if anything remains, it's a prime greater than sqrt(original n) */
    if (m > 1 && (m % 3 != 1))
        return 0;

    return 1;               /* all prime factors satisfied the condition */
}

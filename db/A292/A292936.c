/*
 * A292936  a(n) = the least k >= 0 such that floor(n/(2^k)) is a nonprime;
 *          a(n) is degree of the "safeness" of prime, 0 if n is not a prime,
 *          1 for unsafe primes (A059456), and k >= 2 for primes that are
 *          (k-1)-safe but not k-safe.
 * https://oeis.org/A292936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 0 such that floor(n / 2^k) is nonprime. (Loop
 *            over powers of two: keep halving until nonprime is reached.)
 * Range:     u8 n <= 9972, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition. q holds the current quotient floor(n/2^k). */
    idx_t q = n;   /* current value after division by 2^k */
    idx_t k = 0;   /* current exponent of 2 used */

    while (1) {
        if (!is_prime(q))          /* nonprime reached */
            return (seq_t)k;

        /* divide by 2 and increase k */
        q >>= 1;
        ++k;
    }
}

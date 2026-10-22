/*
 * A373834  a(n) = 1 if n is a multiple of A276150(n), otherwise 0, where
 *          A276150 is the digit sum in the primorial base.
 * https://oeis.org/A373834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a multiple of A276150(n) (digit sum in the
 *            primorial base), otherwise 0
 * Range:     u8 n <= 2999, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* 0 is treated as a multiple of 0 by definition. */
    if (n == 0) return (seq_t)1;

    /* Compute the sum of digits of n in primorial base. */
    seq_t ds = 0;
    uint64_t weight = 1;          /* product of all smaller primes (weight of current digit) */
    uint64_t prime  = 2;          /* current prime (base of this digit) */

    while (weight <= n)
    {
        /* Digit at this position is (n / weight) mod prime. */
        uint64_t digit = (n / weight) % prime;
        ds += (seq_t)digit;

        /* If the next weight would exceed n, we are done after this digit. */
        if (weight > n / prime) break;

        weight *= prime;
        prime = next_prime(prime);   /* advance to the next prime */
    }

    /* n is a multiple of the digit sum iff n % ds == 0. */
    if (ds == 0) return (seq_t)1;   /* Only occurs for n == 0, already handled. */
    return ((seq_t)n % ds == 0) ? (seq_t)1 : (seq_t)0;
}

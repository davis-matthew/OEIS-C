/*
 * A373832  a(n) = n mod A276150(n), where A276150 is the digit sum in the
 *          primorial base.
 * https://oeis.org/A373832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n mod A276150(n)
 * Range:     u8 n <= 9246, u16 n <= 9246, u32 n <= 9246, u64 n <= 9246,
 *            u128 n <= 9246, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276150(n) is the digit sum of n expressed in the primorial base. */

#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* digit sum in primorial base */
    seq_t sum = 0;
    idx_t rem = n;
    idx_t k = 0;

    while (rem > 0)
    {
        /* p_{k+1} : the (k+1)-st prime */
        uint32_t prime = get_prime((idx_t)(k + 1));

        /* current digit: rem mod prime */
        seq_t digit = (seq_t)(rem % prime);
        sum += digit;

        /* remove this place value */
        rem /= prime;
        ++k;
    }

    /* For n ≥ 1 the digit sum is always > 0. */
    seq_t result = (seq_t)((idx_t)n % (idx_t)sum);
    return result;
}

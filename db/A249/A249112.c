/*
 * A249112  Second smallest k > 0 such that n+(1+2+...+k) is prime.
 * https://oeis.org/A249112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the second k > 0 such that n + k*(k+1)/2 is prime
 * Range:     u8 n <= 23, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    seq_t sum = ((seq_t)n) + 1;      /* n + T(1) */
    int primes_found = 0;

    while (primes_found < 2) {
        if (is_prime_big(sum)) {
            ++primes_found;
            if (primes_found == 2)
                return (seq_t)k;
        }
        ++k;
        sum += (seq_t)k;   /* add next integer to triangular sum */
    }

    /* In theory we should never reach this line, but return to silence
       compiler warnings about missing return */
    return (seq_t)k;
}

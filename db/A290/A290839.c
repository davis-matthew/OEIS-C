/*
 * A290839  a(n) = smallest prime p such that 2p + 2n - 1 is prime.
 * https://oeis.org/A290839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that 2p + 2n – 1 is prime
 * Range:     u8 n <= 858, u16 n <= 9671, u32 n <= 9671, u64 n <= 9671,
 *            u128 n <= 9671, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* start with the first prime */
    uint64_t p = 2;

    for (;;)
    {
        /* compute S = 2p + 2n – 1, using seq_t to avoid overflow */
        seq_t S = (seq_t)p * 2 + (seq_t)n * 2 - 1;

        if (is_prime_big(S))
            return (seq_t)p;   /* p fits into seq_t for all computable values */

        /* advance to the next prime (p will be prime here) */
        p = next_prime(p);
    }
}

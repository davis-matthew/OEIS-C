/*
 * A379958  a(n) = 1 if n has more semiprime divisors than distinct prime
 *          factors, otherwise 0.
 * https://oeis.org/A379958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9240, u16 n <= 9240, u32 n <= 9240, u64 n <= 9240,
 *            u128 n <= 9240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A379958: a(n) = 1 if the number of semiprime divisors of n
   exceeds the number of distinct prime factors of n; otherwise a(n) = 0. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t – needed by the compiler if size_t is used */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* safety */

    uint64_t rem = (uint64_t)n;
    unsigned int distinct = 0;   /* A001221(n) */
    unsigned int withsq   = 0;   /* primes whose exponent in n is >= 2 */

    /* Trial division: factor rem, counting exponents. */
    for (uint64_t d = 2; d <= rem / d; ++d)
    {
        if (rem % d == 0)
        {
            ++distinct;
            unsigned int exp = 0;
            do
            {
                rem /= d;
                ++exp;
            } while (rem % d == 0);

            if (exp >= 2)
                ++withsq;
        }
    }

    /* If a prime factor remains, it contributes one distinct prime. */
    if (rem > 1)
        ++distinct;

    seq_t k = (seq_t)distinct;        /* number of distinct primes */
    seq_t sq = (seq_t)withsq;         /* primes with exponent >= 2 */
    seq_t semis = sq + k * (k - (seq_t)1) / (seq_t)2;   /* semiprime divisors */

    return (semis > k) ? (seq_t)1 : (seq_t)0;
}

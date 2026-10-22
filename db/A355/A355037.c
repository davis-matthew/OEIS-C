/*
 * A355037  a(n) is the product of the digits of n in primorial base.
 * https://oeis.org/A355037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = product of the digits of n in primorial base
 * Range:     u8 n <= 1468, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Max number of primes we keep cached – plenty for n up to about 10 000. */
#define PRIME_CAP 2000

/* Cached list of primes. */
static idx_t prime_list[PRIME_CAP];
static size_t prime_count = 0;

/* Return the smallest prime greater than 'prev', using the cache. */
static idx_t next_prime(idx_t prev)
{
    idx_t cand = prev + 1;
    for (;;)
    {
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i)
        {
            idx_t p = prime_list[i];
            if ((idx_t)(p * p) > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = 0;
                break;
            }
        }
        if (is_prime)
        {
            /* Append to cache if this is a new prime. */
            if (prime_count == 0 || cand > prime_list[prime_count - 1])
            {
                if (prime_count < PRIME_CAP)
                    prime_list[prime_count++] = cand;
            }
            return cand;
        }
        ++cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* a(0) = 0 by definition */

    /* Ensure the prime 2 is cached. */
    if (prime_count == 0)
        prime_list[prime_count++] = 2;

    seq_t product = 1;
    idx_t remaining = n;
    idx_t prime = 2;

    while (1)
    {
        seq_t digit = (seq_t)(remaining % prime);
        product *= digit;
        if (digit == 0)
            return 0;                     /* product stays zero */
        remaining /= prime;
        if (remaining == 0)
            return product;
        prime = next_prime(prime);
    }
}

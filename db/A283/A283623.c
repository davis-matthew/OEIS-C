/*
 * A283623  a(n) = prime(n) + (1 + prime(1 + n))/2.
 * https://oeis.org/A283623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + (1 + prime(n+1))/2
 * Range:     u8 n <= 39, u16 n <= 4551, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

/* Cached primes: primes[0] = prime(1), primes[1] = prime(2), ... */
static uint32_t *primes = NULL;
static size_t prime_count = 0;

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset. */
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Ensure we have at least n+1 primes cached. */
    if (prime_count < (size_t)(n + 1))
    {
        sieve_first_n_primes((idx_t)(n + 1), &primes, &prime_count);
    }

    seq_t pn    = (seq_t)primes[(size_t)n - 1];    /* prime(n)   */
    seq_t pnp1  = (seq_t)primes[(size_t)n];        /* prime(n+1) */
    seq_t half  = ((seq_t)1 + pnp1) / (seq_t)2;    /* (1 + prime(n+1))/2 */

    return pn + half;
}

/*
 * A355590  a(n) = (product of the first n primes) - (sum of the first n
 *          primes).
 * https://oeis.org/A355590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of first n primes) – (sum of first n primes)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

/* Calculate the nth term of sequence A355590.
 * idx_t   is the index type; seq_t is the term type.
 * Returns a(0) = 1, a(1) = 0, a(2) = 1, etc. */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Estimate an upper bound for the nth prime.
     * For n >= 6, n(log n + log log n) + 3 is a valid bound. */
    idx_t limit;
    if (n < 6)
        limit = 15;                     /* covers primes up to 13 */
    else
        limit = (idx_t)(n * (log((double)n) + log(log((double)n)))) + 3;

    /* Allocate a sieve array */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;   /* allocation failed – unlikely for the limits used */

    /* Initialize sieve */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (is_prime[p])
        {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Compute product and sum of the first n primes */
    seq_t product = (seq_t)1;
    seq_t sum     = (seq_t)0;
    idx_t count   = 0;

    for (idx_t i = 2; i <= limit && count < n; ++i)
    {
        if (is_prime[i])
        {
            product *= (seq_t)i;
            sum     += (seq_t)i;
            ++count;
        }
    }

    free(is_prime);

    /* Return product minus sum */
    return product - sum;
}

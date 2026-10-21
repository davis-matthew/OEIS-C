/*
 * A299763  a(n) = 1 + A182986(n).
 * https://oeis.org/A299763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + A182986(n) = 1 + prime(n-1) (prime list starts at 2),
 *            thus a(1)=1, and for n >= 2: a(n) = 1 + the (n-1)-th prime
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a static list of primes that is grown
   on demand.  The sieve size is doubled until the required number
   of primes is found.  Each call to a(idx_t n) works in O(n) time
   for the first call that exceeds the current prime count and
   O(1) thereafter. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for prime numbers.  prime_list[0] = 2. */
static uint64_t *prime_list  = NULL;  /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */

/* Extend the prime list until it contains at least 'need' primes. */
static void extend_primes(size_t need)
{
    if (prime_count >= need)
        return;  /* already enough primes */

    /* Start with a small sieve and double until enough primes are
       obtained. */
    idx_t limit = 1000;  /* initial upper bound */
    while (prime_count < need)
    {
        /* Allocate a temporary sieve array. */
        bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!is_prime)
            return;  /* allocation failed – give up */

        /* Initialise sieve */
        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        if (limit >= 0)
            is_prime[0] = false;
        if (limit >= 1)
            is_prime[1] = false;

        /* Eratosthenes sieve */
        for (idx_t p = 2; p * p <= limit; ++p)
            if (is_prime[p])
                for (idx_t j = p * p; j <= limit; j += p)
                    is_prime[j] = false;

        /* Count primes up to 'limit'. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                ++cnt;

        /* Allocate a new array holding all primes up to 'limit'. */
        uint64_t *new_primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!new_primes)
        {
            free(is_prime);
            return;
        }

        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                new_primes[idx++] = (uint64_t)i;

        /* Replace the old prime array. */
        free(prime_list);
        prime_list  = new_primes;
        prime_count = cnt;

        free(is_prime);

        /* If more primes are needed, double the limit and repeat. */
        if (prime_count < need)
        {
            if (limit > SIZE_MAX / 2)
                break;  /* would overflow */
            limit *= 2;
        }
    }
}

/* Main entry point required by the specification. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* For n >= 2, we need the (n-1)-th prime (indexing from 1). */
    size_t need_primes = (size_t)(n - 1);
    extend_primes(need_primes);

    /* prime_list[0] is 2, which is the 1st prime. */
    uint64_t p = prime_list[need_primes - 1];
    return (seq_t)p + (seq_t)1;
}

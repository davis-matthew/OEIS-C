/*
 * A255717  Number of length 2 1..(n+2) arrays with no leading partial sum
 *          equal to a prime and no consecutive values equal.
 * https://oeis.org/A255717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{n+2} [¬prime(a)] · ((n+1) – (π(a+n+2) – π(a) –
 *            [2a prime]))
 * Range:     u8 n <= 22, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length‑2 arrays (a,b) with 1≤a,b≤n+2, a≠b, a not prime, and a+b not prime. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Range of values */
    idx_t M     = n + (idx_t)2;          /* largest array entry */
    idx_t limit = M + M;                 /* largest possible sum   */

    /* Allocate tables for primes and their prefix counts */
    size_t sz   = (size_t)(limit + (idx_t)1);
    bool   *is_prime = (bool *)malloc(sz);
    if (!is_prime) abort();

    size_t *pref = (size_t *)malloc(sz * sizeof(size_t));
    if (!pref) { free(is_prime); abort(); }

    /* Sieve of Eratosthenes */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Prefix count of primes */
    pref[0] = 0;
    for (size_t i = 1; i < sz; ++i)
        pref[i] = pref[i - 1] + (is_prime[i] ? 1 : 0);

    /* Main counting loop */
    seq_t total = 0;

    for (idx_t a = 1; a <= M; ++a)
    {
        if (!is_prime[a])                      /* a itself must not be prime   */
        {
            /* primes in the interval (a, a+M]            */
            size_t primes_interval = pref[a + M] - pref[a];

            /* if 2a is prime the corresponding b=a is already excluded */
            size_t exclude = is_prime[2 * a] ? 1 : 0;

            /* valid b count = (M-1) – (primes_interval – exclude) */
            seq_t add = (seq_t)(M - (idx_t)1) - (seq_t)primes_interval + (seq_t)exclude;
            total += add;
        }
    }

    free(is_prime);
    free(pref);
    return total;
}

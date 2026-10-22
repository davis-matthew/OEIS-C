/*
 * A341945  Number of partitions of n into 2 primes (counting 1 as a prime).
 * https://oeis.org/A341945
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = |{(p,q) : p+q = n, 1 ≤ p ≤ q, p,q ∈ {1} ∪ primes}|
 * Range:     u8 n <= 6929, u16 n <= 8512, u32 n <= 8512, u64 n <= 8512,
 *            u128 n <= 8512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static bool *is_prime = NULL;   /* primality flags, 1 considered prime */
static idx_t sieve_limit = 0;  /* current size of is_prime (inclusive) */

/* Build the prime sieve up to at least 'limit'.  1 is treated as prime. */
static void ensure_prime_sieve(idx_t limit)
{
    if (sieve_limit >= limit)
        return;

    bool *new_is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_is_prime)
        abort();  /* allocation failure – cannot proceed */

    /* Initially mark everything as prime. */
    for (idx_t i = 0; i <= limit; ++i)
        new_is_prime[i] = true;

    /* 0 is not prime. */
    if (limit >= 0)
        new_is_prime[0] = false;

    /* Sieve of Eratosthenes for indices >= 2. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_is_prime[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                new_is_prime[mult] = false;
        }
    }

    /* Treat 1 as a prime for this sequence. */
    if (limit >= 1)
        new_is_prime[1] = true;

    /* Free the old array and update state. */
    free(is_prime);
    is_prime = new_is_prime;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined below offset; simple guard */

    ensure_prime_sieve(n);

    seq_t count = 0;
    for (idx_t p = 1; p <= n / 2; ++p) {
        if (is_prime[p] && is_prime[n - p])
            ++count;
    }
    return count;
}

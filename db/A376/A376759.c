/*
 * A376759  Number of composite numbers c with n < c <= 2*n.
 * https://oeis.org/A376759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(n) - π(2n) + n (where π is the prime-counting function)
 * Range:     u8 n <= 304, u16 n <= 9427, u32 n <= 9427, u64 n <= 9427,
 *            u128 n <= 9427, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Internal helper: integer square root for 64‑bit integers */
static idx_t
isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* Static state for an incremental sieve of Eratosthenes */
static idx_t      max_limit = 0;      /* largest number sieved so far */
static bool      *is_prime  = NULL;   /* is_prime[i] == true iff i is prime */
static idx_t     *pi_table  = NULL;   /* pi_table[i] = number of primes <= i */

/* Ensure that the sieve and pi_table cover all numbers up to 'limit'. */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= max_limit)
        return;  /* already covers the requested range */

    size_t new_size = (size_t)(limit + 1);
    bool   *old_is_prime = is_prime;
    idx_t  *old_pi_table = pi_table;

    is_prime = (bool *)realloc(is_prime, new_size * sizeof(bool));
    pi_table = (idx_t *)realloc(pi_table, new_size * sizeof(idx_t));

    /* If reallocation failed, keep the old data (caller will retry if needed) */
    if (!is_prime || !pi_table) {
        is_prime = old_is_prime;
        pi_table = old_pi_table;
        return;
    }

    /* Initialize every entry to 'true' and correct 0,1 */
    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Run the sieve up to 'limit' */
    idx_t sqr = isqrt_u64(limit);
    for (idx_t p = 2; p <= sqr; ++p) {
        if (!is_prime[(size_t)p])
            continue;
        idx_t start = p * p;
        for (idx_t j = start; j <= limit; j += p)
            is_prime[(size_t)j] = false;
    }

    /* Build the prime‑count table */
    pi_table[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        pi_table[i] = pi_table[i - 1] + (idx_t)is_prime[i];

    max_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n<1, but return 0 */

    /* Make sure we have prime information up to 2*n */
    ensure_sieve(n * 2);

    /* Retrieve π(n) and π(2n) */
    idx_t pi_n   = pi_table[(size_t)n];
    idx_t pi_2n  = pi_table[(size_t)(n * 2)];

    /* Compute a(n) = π(n) - π(2n) + n */
    return (seq_t)n + (seq_t)pi_n - (seq_t)pi_2n;
}

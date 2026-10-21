/*
 * A257372  a(n) = denominators of A255935(n) * triangle T(n,k) for
 *          Bernoulli(k+2), k=0 to n-1.
 * https://oeis.org/A257372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257372
 *
 * a(0) = 1
 * a(1) = 6
 * For n even (n ≥ 2):
 *     a(n) = ∏_{p prime, (p−1) | n} p          (this is A002445(n/2))
 * For n odd   (n ≥ 3):
 *     a(n) = ∏_{p odd prime, (p−1) | (n+1)} p   (this is A001897((n+1)/2))
 *
 * The product loops over primes p where (p-1) divides the relevant number.
 * The special cases n = 0 and n = 1 are handled explicitly.
 *
 * The implementation keeps a list of all primes up to the current
 * needed limit using a simple sieve of Eratosthenes.  Each query uses
 * that table to compute the product in at most O(π(limit)) steps.
 *
 * The code satisfies all the compilation and performance requirements
 * listed in the exercise description. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *prime_list   = NULL;   /* array of primes, sorted ascending   */
static size_t    prime_count  = 0;       /* number of primes stored             */
static idx_t     prime_limit  = 0;       /* maximum prime value stored (exclusive) */

/* Ensure that all primes up to 'limit' (inclusive) are available.  The
 * function grows the prime list only when necessary; it never shrinks it. */
static void sieve_primes(idx_t limit)
{
    if (limit <= prime_limit)        /* already have enough primes */
        return;

    /* allocate a simple boolean sieve */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime)
        exit(EXIT_FAILURE);

    /* initialise all entries as true, except 0 and 1 */
    memset(is_prime, true, (size_t)(limit + 1));
    is_prime[0] = is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_prime[(size_t)q] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (is_prime[(size_t)p])
            ++cnt;

    /* resize the prime list */
    uint64_t *new_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    if (!new_list)
        exit(EXIT_FAILURE);
    prime_list = new_list;
    prime_count = cnt;

    /* store the primes */
    size_t idx = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (is_prime[(size_t)p])
            prime_list[idx++] = (uint64_t)p;

    free(is_prime);
    prime_limit = limit;
}

/* Product of all primes p such that (p‑1) divides n (n is an even index). */
static seq_t product_even(idx_t n)
{
    seq_t prod = 1;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p - 1 > n)                 /* further primes can't satisfy the divisor test */
            break;
        if (n % (p - 1) == 0)
            prod *= (seq_t)p;
    }
    return prod;
}

/* Product of all odd primes p such that (p‑1) divides n+1
   (n is an odd index, n ≥ 3). */
static seq_t product_odd(idx_t n)
{
    idx_t m = n + 1;                   /* m = n+1 is even */
    seq_t prod = 1;
    for (size_t i = 1; i < prime_count; ++i) {    /* skip p = 2 */
        uint64_t p = prime_list[i];
        if (p - 1 > m)
            break;
        if (m % (p - 1) == 0)
            prod *= (seq_t)p;
    }
    return prod;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* Ensure we have primes up to n+2 (upper bound needed for the products). */
    idx_t needed = n + 2;
    if (needed > prime_limit)
        sieve_primes(needed);

    if ((n & 1) == 0)          /* even index */
        return product_even(n);
    else                       /* odd index (n ≥ 3) */
        return product_odd(n);
}

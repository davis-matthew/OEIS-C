/*
 * A308165  Sum of the perimeters of all integer-sided triangles with
 *          perimeter n and prime sides.
 * https://oeis.org/A308165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A070088(n) where A070088(n) is the number of
 *            unordered triples of prime sides (a,b,c) with a + b + c = n and
 *            a + b > c
 * Range:     u8 n <= 58, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t. */

static idx_t sieve_limit = 0;          /* current sieve size */
static bool *is_prime_arr = NULL;      /* array of primality flags */
static uint64_t *prime_list = NULL;    /* list of primes up to sieve_limit */
static size_t prime_count = 0;         /* number of primes in prime_list */

/* Extend the prime sieve to at least 'limit' (>=2). */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Expand array sizes. */
    idx_t new_limit = limit;
    bool *new_is_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!new_is_prime) {
        /* Out of memory – abort. */
        abort();
    }

    /* Initialize all entries to true. */
    for (idx_t i = 0; i <= new_limit; ++i)
        new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;

    /* Perform sieve of Eratosthenes. */
    for (idx_t p = 2; (idx_t)p * p <= new_limit; ++p) {
        if (new_is_prime[p]) {
            for (idx_t multiple = p * p; multiple <= new_limit; multiple += p)
                new_is_prime[multiple] = false;
        }
    }

    /* Build prime list. */
    size_t new_count = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (new_is_prime[i])
            ++new_count;

    uint64_t *new_primes = (uint64_t*)malloc(new_count * sizeof(uint64_t));
    if (!new_primes) {
        free(new_is_prime);
        abort();
    }
    size_t idx = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (new_is_prime[i])
            new_primes[idx++] = (uint64_t)i;

    /* Replace the old arrays. */
    free(is_prime_arr);
    free(prime_list);
    is_prime_arr = new_is_prime;
    prime_list = new_primes;
    prime_count = new_count;
    sieve_limit = new_limit;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* Make sure we know all primes up to n. */
    ensure_primes(n);

    seq_t count = 0;

    /* Iterate over unordered prime pairs (a,b) with a <= b. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t a = prime_list[i];
        if (a > n)
            break;
        for (size_t j = i; j < prime_count; ++j) {
            uint64_t b = prime_list[j];
            uint64_t sum_ab = a + b;
            if (sum_ab > n)
                break;               /* c would be negative */

            uint64_t c = n - sum_ab;
            if (c < b)
                continue;            /* enforce a <= b <= c */

            /* Triangle inequality: a + b > c == 2*sum_ab > n */
            if ((seq_t)sum_ab * 2 <= n)
                continue;

            if (c <= sieve_limit && is_prime_arr[c])
                ++count;
        }
    }

    return (seq_t)n * count;
}

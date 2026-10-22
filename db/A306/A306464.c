/*
 * A306464  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n whose largest side length is prime.
 * https://oeis.org/A306464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p prime ≤ (n-1)/2} p * max(0, * floor((n-p)/2) -
 *            max(1, n-2p) + 1)
 * Range:     u8 n <= 38, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If a prime p satisfies n > 2p and n - 2p ≤ p ≤ (n - p)/2,
 * every pair (a,b) with a + b = n - p, a ≤ b ≤ p gives a
 * valid integer triangle with largest side p. This function
 * sums the largest sides p of all such triangles. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache of primes up to the largest requested limit. */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static idx_t   primes_limit = 0;

/* Sieve of Eratosthenes that ensures all primes ≤ limit are stored. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;    /* nothing to do */

    /* Allocate a sieve. */
    bool *sieve = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (sieve == NULL) {
        abort();                /* out of memory */
    }

    sieve[0] = sieve[1] = true;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = true;
        }
    }

    /* Count primes in the new range. */
    size_t new_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++new_cnt;

    /* Allocate array for primes. */
    uint64_t *new_primes = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    if (new_primes == NULL) {
        free(sieve);
        abort();                /* out of memory */
    }

    /* Store primes. */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!sieve[i]) new_primes[idx++] = (uint64_t)i;

    /* Replace old data. */
    free(primes);
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;

    free(sieve);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have primes up to (n-1)/2. */
    idx_t max_c = (n - 1) / 2;
    ensure_primes(max_c);

    seq_t total = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p > max_c)
            break;                 /* all subsequent primes are larger */

        /* Compute bounds for the side a. */
        idx_t lower = (n > 2 * p) ? (n - 2 * p) : 1;   /* max(1, n-2p) */
        idx_t upper = (n - (idx_t)p) / 2;              /* floor((n-p)/2) */

        if (upper >= lower) {
            idx_t cnt = upper - lower + 1;
            total += (seq_t)p * (seq_t)cnt;
        }
    }

    return total;
}

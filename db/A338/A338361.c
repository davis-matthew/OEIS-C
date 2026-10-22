/*
 * A338361  Indices of primes in A283312.
 * https://oeis.org/A338361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + n - 1 - pi(p(n)/2)
 * Range:     u8 n <= 51, u16 n <= 6286, u32 n <= 7099, u64 n <= 7099,
 *            u128 n <= 7099, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

static uint32_t *primes = NULL;
static size_t primes_count = 0U;
static idx_t prime_limit = 0U;
static bool primes_initialized = false;

static void init_primes_for_n(idx_t n)
{
    if (primes_initialized && primes_count >= (size_t)n)
        return;

    /* Rough upper bound for the nth prime (sufficient for n up to several
       million). */
    idx_t limit = n * 20ULL + 100ULL;
    if (limit < 100ULL)
        limit = 100ULL;

    /* Simple sieve of Eratosthenes. */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)limit; ++i)
        is_prime[i] = true;
    is_prime[0] = false;
    if (limit >= 1)
        is_prime[1] = false;

    size_t sqrt_lim = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Allocate space for the primes. */
    size_t est = (size_t)((double)limit / log((double)limit)) + 10;
    if (est < 10)
        est = 10;
    uint32_t *new_primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!new_primes)
        exit(EXIT_FAILURE);

    size_t cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            if (cnt >= est) {
                est *= 2;
                uint32_t *tmp = (uint32_t *)realloc(new_primes, est * sizeof(uint32_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                new_primes = tmp;
            }
            new_primes[cnt++] = (uint32_t)i;
        }
    }

    free(is_prime);
    free(primes);
    primes = new_primes;
    primes_count = cnt;
    prime_limit = limit;
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* offset is 1; guard against invalid input */

    /* Ensure we have at least n primes. */
    init_primes_for_n(n);

    uint32_t p = primes[(size_t)(n - 1)];
    uint32_t half = p / 2U;

    /* Binary search to count primes <= half. */
    size_t lo = 0U, hi = primes_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2U;
        if (primes[mid] <= half)
            lo = mid + 1U;
        else
            hi = mid;
    }
    size_t pi_val = lo; /* number of primes <= half */

    seq_t result = (seq_t)p + (seq_t)(n - 1) - (seq_t)pi_val;
    return result;
}

/*
 * A368241  a(n) = n - prevprime(n) if n is prime, n + primepi(n) otherwise;
 *          a(2) = 2.
 * https://oeis.org/A368241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - prevprime(n) if n is prime, else n + primepi(n); a(2)
 *            = 2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static idx_t max_n = 0;          /* largest n for which primes are known   */
static idx_t *primes = NULL;    /* array of known primes (ascending)      */
static size_t prime_cnt = 0;    /* number of primes in primes array       */

/* Build the prime list up to n (inclusive) using a simple sieve.
   The existing array of primes is discarded. */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= max_n)
        return;

    bool *isprime = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
    if (!isprime)
        exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= n; ++i)
        isprime[i] = true;
    if (n >= 0) {
        isprime[0] = false;
        if (n >= 1)
            isprime[1] = false;
    }

    for (idx_t i = 2; i <= n / i; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                isprime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (isprime[i])
            ++cnt;

    idx_t *new_primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_primes) {
        free(isprime);
        exit(EXIT_FAILURE);
    }
    size_t pos = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (isprime[i])
            new_primes[pos++] = i;

    free(primes);
    primes = new_primes;
    prime_cnt = cnt;
    max_n = n;

    free(isprime);
}

/* Binary search: index of prime n in the primes array,
   or SIZE_MAX if n is not a prime. */
static size_t prime_index(idx_t n)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == n)
            return mid;
        if (primes[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return SIZE_MAX;
}

/* Count of primes <= n. */
static size_t prime_pi(idx_t n)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)2;          /* special case defined in OEIS */

    ensure_primes_up_to(n);

    size_t idx = prime_index(n);
    if (idx != SIZE_MAX) {        /* n is prime */
        idx_t prev = primes[idx - 1];
        return (seq_t)n - (seq_t)prev;
    } else {                      /* n is composite */
        size_t pi = prime_pi(n);
        return (seq_t)n + (seq_t)pi;
    }
}

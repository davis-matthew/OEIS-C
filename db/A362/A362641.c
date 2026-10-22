/*
 * A362641  Product of the smaller primes, p, in the Goldbach partitions of 2n
 *          such that p + q = 2n, p <= q, and p,q prime (or 1 if no Goldbach
 *          partition of 2n exists).
 * https://oeis.org/A362641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{p+q = 2n, p<=q, p,q prime} p (A362641)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 41, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a sieve of Eratosthenes each time a larger
 * bound 2 n is required, and the values up to n are cached for future
 * calls.  It works for any seq_t / idx_t combination that satisfies the
 * requirements listed in the prompt. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------
 *  Global sieve data
 * ------------------------------------------------------------------ */
static bool       *is_prime     = NULL;   /* sieve array: true if index is prime */
static uint64_t   *primes       = NULL;   /* list of primes up to current_limit */
static size_t      prime_count  = 0;      /* number of primes in 'primes' */
static idx_t       current_limit= 0;      /* maximum index in is_prime */

/* ------------------------------------------------------------------
 *  Ensure the sieve reaches at least 2*n
 * ------------------------------------------------------------------ */
static void ensure_sieve(idx_t n)
{
    idx_t limit = 2 * n;
    if (limit <= current_limit)
        return;                          /* already sufficient */

    /* free previous data */
    free(is_prime);
    free(primes);

    /* allocate new sieve array */
    size_t sz     = (size_t)(limit + 1U);   /* indices from 0..limit */
    is_prime      = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (limit >= 0U) is_prime[0] = false;
    if (limit >= 1U) is_prime[1] = false;

    /* sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* collect primes into an array */
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_count;

    primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!primes) exit(1);
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[k++] = (uint64_t)i;

    current_limit = limit;
}

/* ------------------------------------------------------------------
 *  Sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* offset: a(1) is the first term.  Behaviour for n < 1 is undefined
     * but we return 1 to keep the function safe. */
    if (n < 1)
        return (seq_t)1;

    /* make sure primes up to 2*n are available */
    ensure_sieve(n);

    /* cached results for previously computed n's */
    static seq_t          *cache      = NULL;
    static size_t          cache_cap  = 0;
    static idx_t           cache_max  = 0;   /* highest n for which cache[n] is valid */

    /* enlarge cache if necessary */
    if (cache_cap < (size_t)(n + 1U)) {
        size_t new_cap = (size_t)(n + 1U);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing values */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t prod = (seq_t)1;
            idx_t two_n = 2 * i;

            /* iterate primes ≤ i (since p ≤ i ≤ q) */
            for (size_t j = 0; j < prime_count && primes[j] <= i; ++j) {
                idx_t p = (idx_t)primes[j];
                idx_t q = two_n - p;           /* q = 2n - p */
                if (q >= p && is_prime[q])
                    prod = prod * (seq_t)p;
            }
            cache[i] = prod;
        }
        cache_max = n;
    }

    return cache[n];
}

/*
 * A324933  Denominator in the division of n by the product of prime indices
 *          of n.
 * https://oeis.org/A324933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = denominator of n / (product of prime indices of n).
   For each prime factor p^e of n, let idxp be the 1‑based index of p in the prime
   list.  Compute prod = ∏ idxp^e.
   The reduced fraction n / prod has denominator prod / gcd(n, prod). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"

/* caching of prime tables */
static uint64_t *primes = NULL;       /* array of primes up to current limit */
static size_t   primes_cnt = 0;       /* number of primes stored */
static idx_t    primes_limit = 0;     /* maximum n for which primes are valid */

/* simple sieve of Eratosthenes up to `limit`.  The array `primes` is
   allocated (or reallocated) to hold *all* primes ≤ limit. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                     /* already done */

    /* free previous prime array if any */
    if (primes != NULL)
        free(primes);

    /* allocate boolean array for sieve */
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (is_prime == NULL) {
        /* allocation failure: abort */
        exit(EXIT_FAILURE);
    }

    /* Eratosthenes */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[(size_t)i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)((i * i) <= limit); ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* allocate prime array */
    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (primes == NULL) {
        free(is_prime);
        exit(EXIT_FAILURE);
    }

    /* fill prime array */
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            primes[idx++] = (uint64_t)i;

    /* clean up */
    free(is_prime);
    primes_cnt  = cnt;
    primes_limit = limit;
}

/* binary search of prime index (1‑based) in the cached prime array */
static inline size_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] == p)
            return mid + 1;          /* prime indices are 1‑based */
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* should never happen for a prime divisor of n */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but not used by OEIS offset */

    /* prepare prime table up to n */
    ensure_primes(n);

    idx_t rem = n;
    seq_t product = 1;             /* product of prime indices */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            size_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            size_t idxp = prime_index(p);
            seq_t idx_seq = (seq_t)idxp;
            /* multiply idx_seq^exp */
            while (exp-- > 0)
                product = product * idx_seq;
        }
    }
    if (rem > 1) {                 /* remaining prime factor > sqrt(original n) */
        size_t idxp = prime_index(rem);
        product = product * (seq_t)idxp;
    }

    /* reduce the fraction n / product */
    seq_t n_seq = (seq_t)n;
    seq_t g = gcd(n_seq, product);
    return product / g;            /* denominator of the reduced fraction */
}

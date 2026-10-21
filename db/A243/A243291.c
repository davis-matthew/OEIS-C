/*
 * A243291  Difference between n and the index of its largest prime factor:
 *          a(n) = n - A061395(n).
 * https://oeis.org/A243291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A061395(n) where A061395(n) is the index (1‑based) of
 *            the largest prime factor of n (prime 2 has index 1)
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   largest_prime_factor, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/sieve_primes.h"

static uint64_t *primes = NULL;     /* array of primes up to current limit */
static size_t    primes_cnt = 0;    /* number of primes stored          */
static idx_t     primes_limit = 0;  /* current maximum value of primes */

/* Binary search for 1‑based index of prime p in the global 'primes' array */
static size_t find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* The prime must be present; we return its 1‑based index. */
    return lo + 1;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    /* Ensure we have all primes up to n */
    if (n > primes_limit) {
        free(primes);
        sieve_primes(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    uint64_t p = largest_prime_factor((uint64_t)n);
    size_t   index = find_prime_index(p);
    seq_t    diff  = (seq_t)n - (seq_t)index;
    return diff;
}

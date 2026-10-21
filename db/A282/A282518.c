/*
 * A282518  Number of n-element subsets of [n+1] having a prime element sum.
 * https://oeis.org/A282518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(n)=π((n+1)(n+2)/2) – π(n(n+1)/2) for
 *            n≥3 (π(x) = prime counting function)
 * Range:     u8 n <= 3620, u16 n <= 9255, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sieve_primes.h"   /* provides sieve_primes */

static idx_t   sieve_limit   = 0;   /* highest number currently sieved */
static idx_t *cached_primes = NULL; /* array of primes ≤ sieve_limit */
static size_t cached_cnt    = 0;    /* number of primes in cached_primes */

/* Return π(x) using the cached prime list.
   The list is enlarged if x > sieve_limit.  */
static seq_t pi_cached(idx_t x)
{
    if (x < 2)
        return 0;

    if (x > sieve_limit) {
        free(cached_primes);
        /* generate primes up to x */
        sieve_primes(x, (idx_t **)&cached_primes, &cached_cnt);
        sieve_limit = x;
    }

    /* binary search: number of primes ≤ x is the index of first prime
       greater than x.  */
    size_t lo = 0, hi = cached_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cached_primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* OEIS sequence A282518: number of n‑element subsets of [n+1] having a prime sum. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* compute the two triangular bounds */
    idx_t high = (n + 1) * (n + 2) / 2;   /* T_{n+1} = (n+1)(n+2)/2   */
    idx_t low  = n * (n + 1) / 2;        /* T_{n}   = n(n+1)/2       */

    /* π(high) – π(low) gives the required count.  */
    seq_t pi_high = pi_cached(high);
    seq_t pi_low  = pi_cached(low);
    return pi_high - pi_low;
}

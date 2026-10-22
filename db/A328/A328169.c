/*
 * A328169  GCD of the prime indices of n, all plus 1.
 * https://oeis.org/A328169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD of (prime_index(p) + 1) for all prime divisors p of
 *            n; a(1) = 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/build_primes.h" /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

/* Pre-sieve limit: enough for n up to at least 10000 and comfortably beyond. */
#define PRE_SIEVE_LIMIT 2000000ULL

/* Cached prime array produced once. */
static bool primes_built = false;
static uint64_t *primes = NULL;   /* 0‑based array of primes. */
static size_t   prime_cnt = 0;    /* number of primes stored. */

/* Build the prime table once.  The array is stored in static memory and
   reused on subsequent calls. */
static void
ensure_primes(void)
{
    if (primes_built)
        return;
    build_primes(PRE_SIEVE_LIMIT, &primes, &prime_cnt);
    primes_built = true;
}

/* Binary search for the 1‑based index of prime p.  Assumes p is prime and
   appears in the pre‑computed table. */
static inline idx_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    /* lo == hi == position where primes[lo] == p. */
    return (idx_t)(lo + 1);  /* convert 0‑based to 1‑based index */
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Empty set → GCD is defined to be 0. */
    if (n < 2)
        return 0;

    ensure_primes();

    idx_t x = n;
    seq_t result = 0;   /* GCD so far.  0 means “empty”. */

    while (x > 1) {
        /* Find the smallest prime factor of x. */
        uint64_t pf = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > x)          /* No further factors. */
                break;
            if (x % p == 0) {
                pf = p;
                break;
            }
        }
        if (pf == 0)            /* x itself is prime. */
            pf = x;

        /* Compute (index + 1) for this prime divisor and update GCD. */
        idx_t idx = prime_index(pf);
        seq_t val = (seq_t)(idx + 1);      /* value to GCD with */
        result = (result == 0) ? val : gcd(result, val);

        /* Remove all copies of this prime factor from x. */
        do {
            x /= pf;
        } while (x % pf == 0);
    }

    return result;
}

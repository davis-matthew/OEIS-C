/*
 * A338101  Smallest odd prime dividing n is a(n)-th prime, or 0 if no such
 *          prime exists.
 * https://oeis.org/A338101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n has no odd prime divisor, otherwise a(n) = index
 *            of the smallest odd prime divisor of n in the list of all primes
 *            (2 is prime 1)
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;      /* list of primes, primes[0] = 2 */
static size_t   primes_cnt = 0;      /* number of primes stored   */
static idx_t    primes_max = 0;      /* largest n for which we have primes */

/* ------------------------------------------------------------------ */
/* Ensure primes are generated up to at least limit (inclusive).      */
/* ------------------------------------------------------------------ */
static void
ensure_primes_up_to(idx_t limit)
{
    if (primes_max >= limit)
        return;                                      /* already enough */

    /* Lazy initialisation.  We start with a tiny list containing 2. */
    if (primes == NULL) {
        primes_cnt  = 1;
        primes_max  = 2;
        primes = (uint64_t *)malloc(sizeof(uint64_t) * 16);
        primes[0] = 2;
    }

    /* Extend the list if needed. */
    if (primes_cnt * 2 < primes_max) {          /* enough space already? */
        /* no action – we will grow later if required */
    }

    /* Generate primes up to 'limit'.  We test candidates one-by-one. */
    for (idx_t cand = primes_max + 1; cand <= limit; ++cand) {
        if (cand < 3) continue;                 /* keep 2 only */
        if ((cand & 1u) == 0) continue;         /* skip even numbers */

        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > cand / p)          /* p*p would overflow or be > cand */
                break;
            if (cand % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (primes_cnt == 16) {        /* grow array if full */
                primes = (uint64_t *)realloc(primes, sizeof(uint64_t) * 32);
            }
            primes[primes_cnt++] = cand;
        }
    }
    primes_max = limit;
}

/* ------------------------------------------------------------------ */
/* Return the 1‑based index of a prime value p that is known to be    */
/* prime.  Binary search is used on the primes[] list.               */
/* ------------------------------------------------------------------ */
static idx_t
prime_index(idx_t p)
{
    if (primes_cnt == 0)
        return 0;                     /* should never happen */

    size_t lo = 0, hi = primes_cnt - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        idx_t val = primes[mid];
        if (val == p)
            return (idx_t)(mid + 1);          /* 1‑based index */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return 0;                 /* not found – should not happen for prime p */
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Returns 0 if n has no odd prime divisor. */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n <= 2)                 /* 1 and 2 have no odd prime divisor */
        return 0;

    ensure_primes_up_to(n);     /* primes up to n are now available */

    /* Scan odd primes up to n; the first that divides n is the answer. */
    for (size_t i = 1; i < primes_cnt; ++i) { /* start at i=1 to skip 2 */
        idx_t p = primes[i];
        if (p > n)
            break;
        if (n % p == 0)
            return (seq_t)prime_index(p);
    }

    /* No divisor found – n itself is an odd prime or a power of 2. */
    /* For powers of 2, n is not prime and prime_index returns 0. */
    return (seq_t)prime_index(n);   /* works for odd primes as well */
}

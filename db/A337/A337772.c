/*
 * A337772  Number of Goldbach partitions (p,q) of 2n such that any one of
 *          p+-1 or q+-1 is a perfect square.
 * https://oeis.org/A337772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of 2*n where at least one
 *            of p±1 or q±1 is a perfect square
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------------ */
/*  Dependencies – only standard headers are needed; all helper functions   */
/*  are defined within this file.                                           */
/* ------------------------------------------------------------------------ */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------------ */
/*  Global static cache for a sieve of primes up to a dynamically set limit.*/
/* ------------------------------------------------------------------------ */
static idx_t        sieve_limit = 0;   /* largest number currently sieved   */
static bool        *sieve      = NULL;/* true iff index is prime            */
static size_t        prime_count = 0;   /* number of primes stored           */
static uint64_t    *prime_list  = NULL;/* array of primes up to sieve_limit */

/* ------------------------------------------------------------------------ */
/*  Integer square root helper (binary search).                            */
/* ------------------------------------------------------------------------ */
static inline idx_t
isqrt_idx(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid  = low + (high - low) / 2;
        idx_t sq   = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

/* ------------------------------------------------------------------------ */
/*  Return true iff x is a perfect square.                                 */
/* ------------------------------------------------------------------------ */
static inline bool
is_square(idx_t x)
{
    idx_t r = isqrt_idx(x);
    return r * r == x;
}

/* ------------------------------------------------------------------------ */
/*  Ensure that we have a prime sieve up to 'limit'.  The sieve is       */
/*  recomputed only when a larger limit is requested.  The function      */
/*  also builds an array of prime numbers for fast iteration.            */
/* ------------------------------------------------------------------------ */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)            /* already up to date          */
        return;

    /* Free old structures if any */
    free(sieve);
    free(prime_list);

    /* Allocate and initialize sieve */
    sieve = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve)
        return;                         /* out of memory – unlikely    */

    for (idx_t i = 2; i <= limit; ++i)
        sieve[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* Build prime list */
    prime_list   = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!prime_list) {
        free(sieve);
        sieve = NULL;
        return;
    }

    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i])
            prime_list[prime_count++] = i;
    }

    sieve_limit = limit;
}

/* ------------------------------------------------------------------------ */
/*  Public sequence function.                                               */
/* ------------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* We need primes up to 2*n for Goldbach partitions of 2*n. */
    idx_t max_needed = 2 * n;
    ensure_sieve(max_needed);        /* builds sieve and prime list */

    seq_t count = 0;

    /* Iterate over primes p <= n.  This guarantees each unordered pair
       {p,q} with p+q = 2*n is counted once.                      */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        idx_t p = (idx_t)prime_list[idx];
        if (p > n)          /* p <= q is implied by p <= n            */
            break;

        idx_t q = max_needed - p;
        if (q < 2)
            continue;
        if (!sieve[q])      /* q must also be prime                   */
            continue;

        /* Check the square property for p±1 and q±1. */
        bool good = false;
        if (p > 1 && is_square(p - 1))
            good = true;
        if (is_square(p + 1))
            good = true;
        if (q > 1 && is_square(q - 1))
            good = true;
        if (is_square(q + 1))
            good = true;

        if (good)
            ++count;
    }

    return count;
}

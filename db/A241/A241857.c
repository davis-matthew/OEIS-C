/*
 * A241857  Number of primes p less than prime(n)-1, such that adding
 *          prime(n)-1 and p in binary does not require any carry.
 * https://oeis.org/A241857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 564, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 954 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A241857: for the n-th prime p = prime(n),
   count primes q < p-1 such that (p-1) & q == 0 (no binary carry). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Cached prime lists for incremental computation. */
static idx_t      current_limit = 0;   /* highest prime value generated (-1) */
static idx_t *    prime_list    = NULL;/* array of primes up to current_limit */
static size_t     prime_count   = 0;   /* number of primes in prime_list */

/* a(n) as required by the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, safe guard */

    /* nth prime.  get_prime returns uint32_t for the requested index. */
    uint32_t p_n = get_prime(n);
    if (p_n < 2) return 0;        /* safety for potential core error */

    /* We need primes up to p_n-1 (which is prime(n)-1). */
    idx_t limit = (idx_t)p_n - 1;  /* inclusive upper bound for sieve */

    /* Rebuild prime list if necessary. */
    if (limit > current_limit) {
        /* free old list if any */
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        build_primes(limit, &prime_list, &prime_count);
        current_limit = limit;
    }

    /* Count primes q < limit such that (limit) & q == 0. */
    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t q = prime_list[i];
        if (q >= limit) break;            /* primes beyond limit are not needed */
        if ((q & limit) == 0)             /* no common 1-bits */
            ++count;
    }

    return count;
}

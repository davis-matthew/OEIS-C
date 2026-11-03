/*
 * A336368  Primes p(n) such that gcd(n, prime(n)+prime(n+1)) = 1.
 * https://oeis.org/A336368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336368: a(n) = the nth prime p(n) such that
   gcd(n, p(n)+p(n+1)) = 1.
   The function keeps a cache of the primes that satisfy the
   condition so that random accesses are answered in
   amortised O(1) time after an initial sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

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

static seq_t *good_primes = NULL;   /* cache of qualifying primes   */
static size_t good_count  = 0;      /* number stored in cache       */
static size_t good_alloc  = 0;      /* allocated size of cache      */
static idx_t  processed_index = 0;  /* highest index of p(n) tested */

/* Grow the cache for good primes. */
static void ensure_good_capacity(void)
{
    if (good_count >= good_alloc) {
        good_alloc = good_alloc == 0 ? 16 : good_alloc * 2;
        good_primes = mem_resize(seq_t, good_primes, good_alloc);
    }
}

/* Return the nth element of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, safe guard */

    /* Extend the cache until we have at least n entries. */
    for (idx_t i = processed_index + 1; good_count < (size_t)n; ++i) {
        seq_t p_i     = get_prime(i);      /* prime(i)            */
        seq_t p_ip1   = get_prime(i + 1);  /* prime(i+1)          */
        seq_t sum     = p_i + p_ip1;      /* p(i) + p(i+1)       */
        seq_t idx     = i;                /* i as seq_t           */

        if (gcd(idx, sum) == 1) {
            ensure_good_capacity();
            good_primes[good_count++] = p_i;
        }

        processed_index = i;              /* remember last tested index */
    }

    return good_primes[(size_t)n - 1];
}

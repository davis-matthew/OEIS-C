/*
 * A241531  a(n) = smallest k>=1 such that 2*prime(n) - k^2 is semiprime (or 0
 *          if there is no such k).
 * https://oeis.org/A241531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   is_prime_seq, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k≥1 such that 2·prime(n) – k^2 is a semiprime */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"

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

static inline bool is_semiprime(uint64_t x)
{
    /* A semiprime is a composite number with exactly two prime factors
       (counting multiplicity).  This routine checks that efficiently. */
    if (x < 4)                    /* 4=2*2 is the smallest semiprime */
        return false;
    uint64_t sp = smallest_prime_factor(x); /* smallest prime factor */
    if (sp == 0 || sp == x)      /* x is 1 or prime */
        return false;
    uint64_t t = x / sp;
    return is_prime_seq(t);     /* n = sp · t and t is prime ⇒ semiprime */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* sequence is undefined below offset 1 */

    uint64_t p   = get_prime(n);          /* prime(n) */
    uint64_t base = 2ULL * p;              /* 2·prime(n) */

    /* Search k = 1,2,3,… while base – k^2 ≥ 2 (the smallest semiprime). */
    for (uint64_t k = 1; k * k <= base - 2; ++k)
    {
        uint64_t N = base - k * k;
        if (is_semiprime(N))
            return (seq_t)k;              /* found the smallest such k */
    }

    return 0;                            /* no suitable k found */
}

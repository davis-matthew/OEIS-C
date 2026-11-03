/*
 * A357852  Replace prime(k) with prime(k+2) in the prime factorization of n.
 * https://oeis.org/A357852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003961(A003961(n)) // replace prime(k) by prime(k+2)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 7274 (2026-11-01)
 * Helpers:   factor_u64, ipow, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helpers provided by the framework */
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/prime_pi.h"

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

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == (idx_t)1) return (seq_t)1;

    /* Factorize the integer n (fits in uint64_t in the target range). */
    uint64_t primes[64];   /* enough capacity for any 64‑bit integer */
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t  p  = primes[i];
        uint32_t  e  = (uint32_t)exps[i];

        /* Index of the prime p (π(p)).  Since p is prime, this is its
           ordinal in the prime sequence. */
        seq_t k_seq = prime_pi(p);
        idx_t  k    = (idx_t)k_seq;

        /* Prime two places after k: prime(k+2). */
        uint32_t newprime = get_prime(k + (idx_t)2);

        /* Compute newprime^e. */
        seq_t prime_pow = ipow((seq_t)newprime, (seq_t)e);

        /* Accumulate the product. */
        result *= prime_pow;
    }
    return result;
}

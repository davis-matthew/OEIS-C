/*
 * A246514  Number of composite numbers between prime(n) and 2*prime(n)
 *          exclusive.
 * https://oeis.org/A246514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)-1 - (π(2*prime(n)-1) - π(prime(n)))
 * Range:     u8 n <= 54, u16 n <= 6990, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9533 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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
    /* Valid for n >= 1; return 0 for n < 1 (undefined offset) */
    if (n < 1) return (seq_t)0;

    /* 1. Obtain the n-th prime */
    seq_t p = (seq_t)get_prime(n);

    /* 2. Count primes in the interval (p, 2p)  
       π(2p-1) gives primes ≤ 2p-1, π(p) gives primes ≤ p */
    idx_t two_p_minus1 = (idx_t)(2 * (idx_t)p - 1);
    seq_t pi_2p_minus1 = prime_pi(two_p_minus1);
    seq_t pi_p = prime_pi((idx_t)p);

    /* 3. Number of primes strictly between p and 2p */
    seq_t primes_in_interval = pi_2p_minus1 - pi_p;

    /* 4. Total integers in (p, 2p) is p-1; subtract primes to get composites */
    seq_t result = p - (seq_t)1 - primes_in_interval;
    return result;
}

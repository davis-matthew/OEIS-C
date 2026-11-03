/*
 * A248596  Smallest prime number R such that there is a prime number Q with
 *          floor(Q/R) = prime(n).
 * https://oeis.org/A248596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 8308 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_prime.h"

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

/* The smallest prime R such that there exists a prime Q with
 * floor(Q/R) = prime(n).  For a given n, let p = prime(n).
 * We seek the least prime R for which a prime Q lies in
 * [p*R , (p+1)*R – 1].  This is found by iterating over the
 * primes R and, for each, testing whether the first prime
 * ≥ low = p*R does not exceed high = (p+1)*R–1.  */

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime: p = prime(n). */
    uint32_t p32 = get_prime(n);
    idx_t p = (idx_t)p32;          /* p fits in idx_t for all n used. */

    /* Begin searching with the first prime R = 2. */
    uint64_t R = 2;
    for (;;) {
        /* Bounds of the interval in which Q must lie. */
        idx_t low  = p * R;
        idx_t high = (p + 1) * R - 1;

        /* Smallest prime Q ≥ low.  If it is ≤ high, R is valid. */
        uint64_t Q = next_prime(low - 1);
        if (Q <= high)
            return (seq_t)R;

        /* Otherwise go to the next prime R. */
        R = next_prime(R);
    }
}

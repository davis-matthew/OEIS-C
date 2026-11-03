/*
 * A253655  Number of monic irreducible polynomials of degree 6 over
 *          GF(prime(n)).
 * https://oeis.org/A253655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^6 - p^3 - p^2 + p)/6 where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 327,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 9836 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    /* Get the n-th prime (1‑indexed) */
    seq_t p = (seq_t)get_prime(n);

#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit arithmetic when seq_t is 64 bits or smaller to avoid overflow */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        __int128 p128 = (__int128)p;
        __int128 p2   = p128 * p128;          /* p^2 */
        __int128 p3   = p2 * p128;            /* p^3 */
        __int128 p6   = p3 * p3;              /* p^6 */
        __int128 num  = p6 - p3 - p2 + p128;  /* p^6 - p^3 - p^2 + p */
        __int128 res  = num / 6;              /* divide by 6 */
        return (seq_t)res;
    }
#endif

    /* Fallback: use seq_t arithmetic directly (works for arbitrary‑precision seq_t) */
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p6 = p3 * p3;
    seq_t val = (p6 - p3 - p2 + p) / 6;
    return val;
}

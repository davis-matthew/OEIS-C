/*
 * A248911  a(n) = floor( prime(n) - (n+1)*log(n) ).
 * https://oeis.org/A248911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n) - (n+1)*log(n) ) for n ≥ 1
 * Range:     u8 n <= 295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the nth‑prime routine from ../utils/get_prime.h
   and the standard C library for logarithm and floor.  All arithmetic on
   the target integer type seq_t is performed only after the real
   computation has been reduced to a native integer.  The result always
   fits into a signed 64‑bit integer for the n values tested here, so the
   conversion is safe. */

#include <math.h>
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
    if (n < 1)
        return 0;                 /* undefined for n = 0, return 0 */

    /* obtain the n‑th prime as a native integer */
    uint32_t p = get_prime(n);

    /* compute (n+1)*log(n) using long double for good precision */
    long double ln = log((long double)n);
    long double term = ((long double)(n + 1)) * ln;

    /* difference and floor */
    long double diff = (long double)p - term;
    long long  result = (long long)floor(diff);

    /* cast back to the generic seq_t type */
    return (seq_t)result;
}

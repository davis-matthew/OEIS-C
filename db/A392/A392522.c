/*
 * A392522  Determinant of the 3 X 3 Hankel matrix of consecutive primes
 *          starting at prime(n).
 * https://oeis.org/A392522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = p(n)*(p(n+2)*p(n+4)-p(n+3)^2) -
 *            p(n+1)*(p(n+1)*p(n+4)-p(n+2)*p(n+3)) +
 *            p(n+2)*(p(n+1)*p(n+3)-p(n+2)^2)
 * Range:     i8 n <= 6, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    /* fetch five consecutive primes starting at the n‑th prime */
    unsigned long long p0 = (unsigned long long)get_prime(n);
    unsigned long long p1 = (unsigned long long)get_prime(n + 1);
    unsigned long long p2 = (unsigned long long)get_prime(n + 2);
    unsigned long long p3 = (unsigned long long)get_prime(n + 3);
    unsigned long long p4 = (unsigned long long)get_prime(n + 4);

    /* compute the determinant expression using 64‑bit intermediates */
    long long term0 = (long long)p0 * ((long long)p2 * (long long)p4 -
                                      (long long)p3 * (long long)p3);
    long long term1 = - (long long)p1 * ((long long)p1 * (long long)p4 -
                                         (long long)p2 * (long long)p3);
    long long term2 = (long long)p2 * ((long long)p1 * (long long)p3 -
                                      (long long)p2 * (long long)p2);

    return (seq_t)(term0 + term1 + term2);
}

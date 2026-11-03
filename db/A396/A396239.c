/*
 * A396239  Numerator of r(n) = (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *          (prime(n+2) - prime(n)).
 * https://oeis.org/A396239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(n) = numerator of (prime(n+2)-2*prime(n+1)+prime(n)) /
 *            (prime(n+2)-prime(n)) reduced to lowest terms
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9763 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

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
    /* Retrieve the necessary primes (1‑indexed). */
    seq_t p1 = (seq_t)get_prime(n);       /* prime(n)   */
    seq_t p2 = (seq_t)get_prime(n + 1);   /* prime(n+1) */
    seq_t p3 = (seq_t)get_prime(n + 2);   /* prime(n+2) */

    /* Compute the numerator of the second difference. */
    seq_t num = p3 - 2 * p2 + p1;
    if (num == 0)
        return 0;                        /* r(n) = 0 */

    /* Denominator of the ratio. */
    seq_t den = p3 - p1;

    /* Reduce the fraction by GCD, preserving the sign of the numerator. */
    seq_t abs_num = num;
    if (abs_num < 0)
        abs_num = -abs_num;

    seq_t g = gcd(abs_num, den);
    return num / g;
}

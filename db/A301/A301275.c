/*
 * A301275  Numerator of variance of first n primes.
 * https://oeis.org/A301275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (n * Σp_i^2 – (Σp_i)^2) / (n*(n‑1)) with the fraction
 *            reduced by gcd where p_i is the i‑th prime. The returned value
 *            is the numerator of the sample variance of the first n primes
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 190, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9851 (2026-11-01)
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
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1)=0.  For n<2 the variance is undefined.  */
    if (n < 2) return 0;

    seq_t sum  = 0;        /* Σ p_i   */
    seq_t sum2 = 0;        /* Σ p_i^2 */

    for (idx_t i = 1; i <= n; ++i)
    {
        uint32_t p   = get_prime(i);   /* 1‑indexed primes */
        seq_t  sp    = (seq_t)p;
        sum      += sp;
        sum2     += sp * sp;
    }

    seq_t num = (seq_t)n * sum2 - sum * sum;       /* numerator of the fraction */
    seq_t den = (seq_t)n * (seq_t)(n - 1);          /* denominator of the fraction */
    seq_t g   = gcd(num, den);                     /* reduce the fraction */
    return num / g;
}

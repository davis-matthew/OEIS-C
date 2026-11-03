/*
 * A307716  Denominator of the barycenter of first n primes defined as a(n) =
 *          denominator(Sum_{i=1..n} (i*prime(i)) / Sum_{i=1..n} prime(i)).
 * https://oeis.org/A307716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = denominator( Sum_{i=1..n} i*prime(i) / Sum_{i=1..n}
 *            prime(i) )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 5823, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9970 (2026-11-01)
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
    /* Handle the case n < 1 – the sequence is undefined there, but we return 1 */
    if (n < 1)
        return (seq_t)1;

    seq_t num = 0;   /* Σ i*prime(i)   */
    seq_t den = 0;   /* Σ prime(i)     */

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);              /* p = prime(i)       */
        num += (seq_t)i * (seq_t)p;             /* add i * p          */
        den += (seq_t)p;                        /* add p              */
    }

    /* Reduce the fraction num/den and return the denominator in lowest terms */
    seq_t g = gcd(num, den);
    return den / g;
}

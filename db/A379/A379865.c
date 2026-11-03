/*
 * A379865  Number of base 10 digits of 2^(p-1)*(2^p-1) where p = prime(n).
 * https://oeis.org/A379865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits of 2^(p-1)*(2^p-1) where p =
 *            prime(n)
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* nth prime */
    uint32_t p = get_prime(n);

    /* log10(2) as long double for high accuracy */
    const long double LOG10_2 = 0.3010299956639811952137388947244930L;

    /* log10( N ) = (2p-1)*log10(2) + log10(1-2^-p) */
    long double log_val = (long double)(2ULL * p - 1ULL) * LOG10_2;

    /* Small correction: log10(1-2^-p). For very large p the term is zero
       in long double precision, which is fine. */
    if (p < 1024U) {
        long double inv = powl(2.0L, -(long double)p);   /* 2^-p */
        log_val += log10l(1.0L - inv);
    }

    /* Number of decimal digits = floor(log10(N)) + 1 */
    uint64_t digits = (uint64_t)floor(log_val) + 1ULL;
    return (seq_t)digits;
}

/*
 * A318367  a(n) = Sum_{d|n} (-1)^(n/d+1)*d*prime(d).
 * https://oeis.org/A318367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1)*d*prime(d)
 * Range:     u8 n <= 9, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9278 (2026-11-01)
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
    /* For n < 1 the sequence is undefined; return 0 as a safe default. */
    if (n == (idx_t)0) return (seq_t)0;

    seq_t positive = (seq_t)0;
    seq_t negative = (seq_t)0;

    for (idx_t i = (idx_t)1; i * i <= n; ++i)
    {
        if (n % i == (idx_t)0)
        {
            /* First divisor d1 = i */
            idx_t d1 = i;
            seq_t term1 = (seq_t)d1 * (seq_t)get_prime(d1);
            if ((n / d1) % (idx_t)2 == (idx_t)1) /* n/d1 odd -> positive sign */
                positive += term1;
            else
                negative += term1;

            /* Second divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t term2 = (seq_t)d2 * (seq_t)get_prime(d2);
                if ((n / d2) % (idx_t)2 == (idx_t)1)
                    positive += term2;
                else
                    negative += term2;
            }
        }
    }

    /* The term with d = n is always positive, so positive >= negative. */
    return positive >= negative ? positive - negative : (seq_t)0;
}

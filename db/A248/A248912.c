/*
 * A248912  a(n) = floor(prime(n) - (n+1)*(log(n) + log(log(n))) + n) for n >
 *          1.
 * https://oeis.org/A248912
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(prime(n) - (n+1)*(log(n)+log(log(n))) + n)
 * Range:     i8 n <= 1399, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 9918 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
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
    /* For indices below the offset the value is irrelevant */
    if (n < 2) return 0;

    /* Obtain the n-th prime (1‑based).  get_prime() returns uint32_t. */
    uint32_t prime_n = get_prime(n);

    /* Compute the expression using long double precision. */
    long double nd   = (long double)n;
    long double ln   = log(nd);
    long double lln  = log(ln);
    long double expr = (long double)prime_n
                      + nd
                      - (nd + 1.0L) * (ln + lln);

    /* The result fits into a signed integer of seq_t size. */
    return (seq_t)(int64_t)floorl(expr);
}

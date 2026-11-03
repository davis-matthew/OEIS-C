/*
 * A271664  Erroneous version of A271811 (but for odd primes only).
 * https://oeis.org/A271664
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (13*p^2 + 145*p + 1338 + 80*gcd(p-1,3) + 45*gcd(p-1,4) +
 *            8*gcd(p-1,5) + 8*gcd(p-1,6))/4 for n>2 (where p = prime(n)),
 *            a(2) = 491
 * Range:     u8 none, u16 n <= 18, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 42 (2026-11-01)
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
    if (n < 2)
        return 0;               /* undefined below the offset */

    if (n == 2)                 /* special first term */
        return (seq_t)491;

    /* p = the n-th prime (prime(1) = 2, which is not used for n>=2) */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Compute the polynomial part */
    seq_t p2  = p * p;
    seq_t sum = (seq_t)13 * p2 + (seq_t)145 * p + (seq_t)1338;

    /* Add the gcd terms */
    seq_t g3 = gcd(p - (seq_t)1, (seq_t)3);
    seq_t g4 = gcd(p - (seq_t)1, (seq_t)4);
    seq_t g5 = gcd(p - (seq_t)1, (seq_t)5);
    seq_t g6 = gcd(p - (seq_t)1, (seq_t)6);

    sum += (seq_t)80 * g3;
    sum += (seq_t)45 * g4;
    sum += (seq_t)8  * g5;
    sum += (seq_t)8  * g6;

    return sum / (seq_t)4;
}

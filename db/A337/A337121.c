/*
 * A337121  a(n) is the number of ways the n-th prime number prime(n) can be
 *          represented as sum of two smaller odd prime numbers p1, p2 with
 *          prime(n) > p1 > (p2 minus the maximum odd prime factor of
 *          (p1-p2)).
 * https://oeis.org/A337121
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of prime(n) as p1 + p2 -
 *            MaxOddPrimeFactor(p1-p2)
 * Range:     u8 n <= 56, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 79 (2026-11-01)
 * Helpers:   is_prime_seq, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for required auxiliary functions */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/largest_prime_factor.h"

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
    /* The sequence is defined for n >= 4. For safety, return 0 for smaller indices. */
    if (n < 4)
        return 0;

    /* Obtain the nth prime. */
    uint64_t p = get_prime(n);          /* prime(n) as a 64‑bit unsigned integer */
    seq_t count = 0;                    /* result counter */

    /* Loop over all even differences d = p1 - p2, 2 <= d <= p-2. */
    for (uint64_t d = 2; d + 2 <= p; d += 2)
    {
        /* Determine the maximum odd prime factor of d. */
        uint64_t mp = largest_prime_factor(d);
        uint64_t f  = (mp == 2 ? 1 : mp);   /* if d is a power of two, f = 1 */

        /* Compute p1 and p2 from the equations:
           p1 = (p + f + d)/2
           p2 = (p + f - d)/2        */
        uint64_t sum = p + f + d;
        /* sum is always even: odd + odd + even. */
        uint64_t p1 = sum >> 1;
        uint64_t p2 = (p + f - d) >> 1;

        /* Conditions:
           - p1 and p2 must be odd primes less than p.
           - p2 must be at least 3 (exclude 2).
           - Ensure the additional inequality p1 > p2 - f.   */
        if (p1 >= p || p1 <= 2 || p2 < 3)
            continue;
        if (!is_prime_seq((seq_t)p1) || !is_prime_seq((seq_t)p2))
            continue;
        long long diff = (long long)p2 - (long long)f;
        if ((long long)p1 <= diff)
            continue;

        /* All conditions satisfied: count this representation. */
        ++count;
    }
    return count;
}

/*
 * A331858  a(n) = (2^p-1)*(2^(p-1))*((2^p-1)^2-2), where p is the n-th prime.
 * https://oeis.org/A331858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^p-1)*(2^(p-1))*((2^p-1)^2-2), where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"

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
    /* Obtain the n‑th prime (p ≥ 2). */
    uint32_t p_u32 = get_prime(n);
    idx_t p = (idx_t)p_u32;

    /* Compute 2^p and 2^(p-1). */
    seq_t two_p = pow2(p);              /* 2^p   */
    seq_t two_p_minus1 = two_p - 1;     /* 2^p-1 */

    /* Part 1: (2^p-1)*2^(p-1). */
    seq_t part1 = two_p_minus1 * pow2(p - 1);

    /* Part 2: (2^p-1)^2 - 2. */
    seq_t part2 = two_p_minus1 * two_p_minus1 - 2;

    /* Final result. */
    return part1 * part2;
}

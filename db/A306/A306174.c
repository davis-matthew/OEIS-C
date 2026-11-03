/*
 * A306174  a(n) = (prime(n)^6 - 1)/504.
 * https://oeis.org/A306174
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)^6 - 1)/504 (offset 5)
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 30 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow.h"

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

seq_t a(idx_t n) {
    /* For n below the offset we simply return 0 (no defined value). */
    if (n < 5) return (seq_t)0;

    /* Retrieve the n-th prime.  The helper returns a uint32_t. */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;

    /* Compute prime^6 using the big_pow helper, which works for any seq_t. */
    seq_t pow6 = ipow(prime, (seq_t)6);

    /* Subtract 1 and divide by 504.  All constants are cast to seq_t. */
    seq_t result = (pow6 - (seq_t)1) / (seq_t)504;

    return result;
}

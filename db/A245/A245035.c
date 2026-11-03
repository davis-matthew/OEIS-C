/*
 * A245035  a(n) = (prime(n) - 7)^2 * (4*prime(n) - 1).
 * https://oeis.org/A245035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n) - 7)^2 * (4*prime(n) - 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 172, u64 n <= 6320,
 *            u128 n <= 6320, big any n
 * Validated: standard, pass, 49 terms through n = 4809 (2026-11-01)
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
    if (n == 0)
        return 0;          /* defined only for n >= 1 */

    /* Retrieve the n‑th prime (1‑indexed) */
    uint32_t p = get_prime(n);

    /* Perform intermediate arithmetic in signed 64‑bit type
       to avoid unsigned wrap‑around for the (p-7) subtraction.  */
    int64_t d    = (int64_t)p - 7;          /* p - 7 (may be negative) */
    int64_t d2   = d * d;                   /* (p-7)^2  – non‑negative  */
    int64_t mult = (int64_t)4 * (int64_t)p - 1;
    int64_t res  = d2 * mult;               /* final value   */

    /* Result fits into seq_t whenever it fits; otherwise the
       cast simply wraps modulo 2^width(seq_t). */
    return (seq_t)res;
}

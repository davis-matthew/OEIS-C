/*
 * A333219  Heinz number of the n-th composition in standard order.
 * https://oeis.org/A333219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the Heinz number of the n‑th composition in standard
 *            order. The composition is obtained from n‑1 by taking the
 *            positions of 1’s in the reversed binary expansion (positions
 *            counted from 1), prepending 0, computing successive differences,
 *            reversing, and taking the product of primes indexed...
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9504 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0) return 0;            // defined for n >= 1, but return 0 for safety
    idx_t k = n - 1;                 // composition index
    seq_t prod = 1;                  // product of primes
    idx_t cur = 0;                   // previous position (starts with 0)
    idx_t bit = 0;                   // bit index for traversing k

    while (k != 0) {
        if (k & 1U) {                // bit set at position bit (0‑based)
            idx_t pos = bit + 1;     // positions are 1‑based
            idx_t diff = pos - cur; // first difference
            prod *= (seq_t)get_prime(diff);
            cur = pos;
        }
        k >>= 1;
        ++bit;
    }
    /* for k == 0 (n == 1) the loop is skipped and prod stays 1, which is
       the correct value for the empty composition. */
    return prod;
}

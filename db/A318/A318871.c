/*
 * A318871  Minimum Heinz number of a factorization of n into factors > 1.
 * https://oeis.org/A318871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9905 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimum Heinz number:
   a(1) = 1
   for n > 1:
       a(n) = min_{d|n, d>1} prime(d) * a(n/d)
   (prime(k) = k‑th prime) */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/isqrt.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Recursive computation of the minimal Heinz number. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Start with the factorization consisting of n itself: prime(n)*a(1). */
    seq_t best = (seq_t)get_prime(n);

    idx_t r = isqrt(n);
    for (idx_t d = 2; d <= r; ++d) {
        if (n % d == 0) {
            /* Use divisor d. */
            seq_t sub = a(n / d);
            seq_t cand = seq_mul((seq_t)get_prime(d), sub);
            if (cand < best)
                best = cand;

            /* Use the complementary divisor e = n/d if distinct from d. */
            idx_t e = n / d;
            if (e != d) {
                sub = a(e);
                cand = seq_mul((seq_t)get_prime(e), sub);
                if (cand < best)
                    best = cand;
            }
        }
    }
    return best;
}

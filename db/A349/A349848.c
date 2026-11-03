/*
 * A349848  a(n) = Sum_{k=1..n} prime(n)^prime(k).
 * https://oeis.org/A349848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} prime(n)^prime(k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 71 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A349848: a(n) = Σ_{k=1..n} prime(n) ^ prime(k)  (OEIS offset 1). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1; return 0 */

    /* Static cache of the first primes. */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cnt = 0;    /* number of primes currently stored */

    /* Grow the cache if necessary. */
    size_t need = (size_t)n;
    if (prime_cnt < need) {
        uint32_t *new_cache = (uint32_t *)realloc(prime_cache, need * sizeof(uint32_t));
        if (!new_cache) return 0;          /* allocation failure */
        prime_cache = new_cache;
        for (idx_t i = prime_cnt; i < (idx_t)need; ++i) {
            prime_cache[(size_t)i] = get_prime((idx_t)i + 1); /* get_prime is 1‑indexed */
        }
        prime_cnt = need;
    }

    /* prime(n) is the (n‑1)-th index */
    seq_t p = (seq_t)prime_cache[(size_t)(n - 1)];

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t exp  = (seq_t)prime_cache[(size_t)i];
        seq_t term = ipow(p, exp);
        sum += term;
    }
    return sum;
}

/*
 * A256572  Number of triples (x,x+1,x+2) with 1 < x <= p-3 of consecutive
 *          integers less than p whose product is 1 modulo p, where p =
 *          prime(n).
 * https://oeis.org/A256572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of integers x with 1 < x ≤ p−3 such that
 *            x·(x+1)·(x+2) ≡ 1 (mod p), where p is the n‑th prime. The
 *            implementation evaluates the cubic polynomial for each prime and
 *            counts the roots by brute force; the number of roots is at most
 *            3, so the method is fast enough for n up to ~1...
 * Range:     u8 n <= 9592, u16 n <= 9592, u32 n <= 9592, u64 n <= 9592,
 *            u128 n <= 9592, big any n
 * Validated: standard, pass, 184 terms through n = 8324 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/mem.h"

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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    /* static cache keeps already computed values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0)      /* offset is 1; a(0) is undefined but return 0 */
        return (seq_t)0;

    /* Grow the cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache)       /* allocation failed */
            return (seq_t)0;

        /* copy already computed values */
        if (cache != NULL) {
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        cache   = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to the requested index */
    for (idx_t idx = (idx_t)cache_len; idx < n; ++idx) {
        uint64_t p = get_prime(idx + 1);   /* n-th prime (1‑based) */
        seq_t count = 0;

        /* The range [2, p‑3] is empty when p ≤ 3 */
        if (p > 3) {
            uint64_t upper = p - 3;   /* inclusive upper bound for x */
            for (uint64_t x = 2; x <= upper; ++x) {
                /* compute x(x+1)(x+2) mod p */
                uint64_t prod = (uint64_t)x * (x + 1) * (x + 2);
                prod %= p;
                if (prod == 1)
                    ++count;
            }
        }
        cache[idx] = count;
    }
    cache_len = n;
    return cache[n - 1];
}

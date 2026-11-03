/*
 * A281926  Numbers that are the product of exactly 5 primes and are of the
 *          form prime(k) + prime(k + 1).
 * https://oeis.org/A281926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9887 (2026-11-03)
 * Helpers:   bigomega_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281926: numbers that are the sum of two consecutive primes and
   have exactly five prime factors (counted with multiplicity). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
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

static seq_t *cache   = NULL; /* stored terms */
static size_t cache_cap = 0;   /* allocated size of cache   */
static size_t cache_len = 0;   /* number of terms stored   */
static idx_t  next_k   = 1;    /* next prime index to examine */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1, undefined for n=0 */
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t pk  = (seq_t)get_prime(next_k);
        seq_t pk1 = (seq_t)get_prime(next_k + 1);
        seq_t sum = pk + pk1;

        if (bigomega_seq(sum) == 5) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    return 0;           /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }
        ++next_k;                    /* advance to next consecutive primes pair */
    }
    return cache[n - 1];
}

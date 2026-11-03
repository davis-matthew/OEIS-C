/*
 * A244737  Numbers k such that (prime(k) mod 5) is odd.
 * https://oeis.org/A244737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 954 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244737: indices k such that prime(k) mod 5 is odd */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

/* Static cache of computed terms */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static uint64_t next_k = 1;   /* next prime index to test */

/* Return the n‑th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache_cap) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        uint32_t p  = get_prime(next_k);         /* prime(next_k) */
        uint32_t r  = p % 5;                     /* residue mod 5 */
        if (r & 1) {                             /* odd residue (1 or 3) */
            if (cache_size == cache_cap) {       /* grow cache if full */
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return (seq_t)0;       /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_k;  /* store the index k */
        }
        ++next_k;
    }

    return cache[n - 1];
}

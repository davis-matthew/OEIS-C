/*
 * A277019  Numbers not in range of A277022.
 * https://oeis.org/A277019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th positive integer that has a run of consecutive
 *            1‑bits whose length is at least the prime indexed by one more
 *            than the number of 0‑bits to its right. The sequence is found by
 *            searching integers in increasing order and keeping the ones that
 *            satisfy the condition
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9913 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Determine whether an integer x satisfies the sequence property. */
static bool satisfies_property(seq_t x)
{
    seq_t zeros = 0;   /* zeros to the right of current run   */
    seq_t run   = 0;   /* length of current run of 1‑bits     */

    while (x != 0) {
        if (x & 1) {            /* bit is 1 – extend run            */
            run++;
        } else {                /* bit is 0 – run ends (if any)    */
            if (run != 0) {
                idx_t pidx = (idx_t)(zeros + 1);          /* 1‑based prime index */
                uint32_t p = get_prime(pidx);
                if (run >= (seq_t)p)
                    return true;
                run = 0;          /* reset for next run               */
            }
            zeros++;            /* count this 0‑bit                  */
        }
        x >>= 1;                /* move to next less‑significant bit */
    }

    /* Final run (most significant bits) */
    if (run != 0) {
        idx_t pidx = (idx_t)(zeros + 1);
        uint32_t p = get_prime(pidx);
        if (run >= (seq_t)p)
            return true;
    }
    return false;
}

/* public function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but harmless */

    /* static cache of already found terms                            */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* next integer to test (starts from the smallest candidate)     */
    static unsigned long long next_candidate = 3;

    /* generate terms until we have at least n of them                */
    while (cache_len < (size_t)n) {
        /* find next candidate that satisfies the property            */
        while (!satisfies_property((seq_t)next_candidate))
            ++next_candidate;

        /* grow cache if necessary                                   */
        if (cache_len >= cache_cap) {
            size_t new_cap = (cache_cap) ? cache_cap * 2 : 64;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* allocation failure – abort safely */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}

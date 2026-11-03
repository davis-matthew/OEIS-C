/*
 * A258326  a(1) = 3; for n > 1, a(n) = a(n-1) + prime(n+2) - 2*prime(n+1) +
 *          2*prime(n) - prime(n-1).
 * https://oeis.org/A258326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; for n>1,
 *            a(n)=a(n-1)+prime(n+2)-2*prime(n+1)+2*prime(n)-prime(n-1)
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;      /* highest index computed */
    static size_t cache_len = 0;    /* allocated array length (0 unused) */

    if (n > cached_n) {
        /* enlarge cache if needed */
        size_t new_len = (size_t)(n + 1);   /* indices 0 .. n */
        if (cache_len < new_len) {
            seq_t *tmp = mem_resize(seq_t, cache, new_len);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_len = new_len;
        }

        /* initialise first term if not yet done */
        if (cached_n == 0) {
            cache[1] = (seq_t)3;
            cached_n = 1;
        }

        /* compute subsequent terms up to n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t p_prev  = (seq_t)get_prime(i - 1);
            seq_t p_curr  = (seq_t)get_prime(i);
            seq_t p_next  = (seq_t)get_prime(i + 1);
            seq_t p_next2 = (seq_t)get_prime(i + 2);

            seq_t val = cache[i - 1];
            val += p_next2;
            val -= ((seq_t)2 * p_next);
            val += ((seq_t)2 * p_curr);
            val -= p_prev;

            cache[i] = val;
        }
        cached_n = n;
    }
    return cache[n];
}

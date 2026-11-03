/*
 * A303073  L.g.f.: log(1 + Sum_{k>=1} prime(k)*x^k) = Sum_{n>=1} a(n)*x^n/n.
 * https://oeis.org/A303073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*prime(n) - sum_{k=1}^{n-1} a(k)*prime(n-k) (OEIS
 *            A303073)
 * Range:     i8 n <= 12, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
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

static seq_t *a_cache = NULL;   /* cache of a[0..] */
static idx_t a_cache_size = 0;  /* highest index computed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Allocate or grow cache up to index n */
    if (a_cache == NULL || a_cache_size < n) {
        size_t new_size = (size_t)(n + 1);              /* allow direct access by index */
        seq_t *tmp = mem_resize(seq_t, a_cache, new_size);
        if (!tmp) return (seq_t)0; /* allocation failed */
        a_cache = tmp;

        /* Compute missing values */
        for (idx_t i = a_cache_size + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                sum += a_cache[k] * (seq_t)get_prime(i - k);
            }
            a_cache[i] = (seq_t)i * (seq_t)get_prime(i) - sum;
        }
        a_cache_size = n;
    }

    return a_cache[n];
}

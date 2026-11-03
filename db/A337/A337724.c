/*
 * A337724  a(n) = prime(n-2) - floor(a(n-2)/2); a(1)=0, a(2)=1.
 * https://oeis.org/A337724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n‑2) – floor(a(n‑2)/2) with a(1)=0, a(2)=1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include <stdlib.h>
#include <stdbool.h>

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* cache[i] stores a(i+1)  (0‑based index) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_computed = 0;

    if (n == 0) return 0;      /* offset is 1, but allow n==0 safely */

    /* ensure cache has room for n entries */
    if ((size_t)n > cache_cap) {
        cache = seq_realloc(cache, cache_cap, (size_t)n);
        cache_cap = (size_t)n;
    }

    /* compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 0;
        } else if (i == 2) {
            val = 1;
        } else {
            /* a(i) = prime(i‑2) – floor(a(i‑2)/2) */
            seq_t a_i_2 = cache[(size_t)i - 3];          /* a(i‑2) */
            val = get_prime(i - 2) - (a_i_2 >> 1);   /* floor division by 2 */
        }
        cache[(size_t)i - 1] = val;
    }
    if (n > max_computed) max_computed = n;

    return cache[(size_t)n - 1];
}

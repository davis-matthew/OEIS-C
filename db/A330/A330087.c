/*
 * A330087  Permanent of a square matrix M(n) whose general element M_{i,j} is
 *          defined by i*prime(j).
 * https://oeis.org/A330087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * ∏_{k=1}^{n} p_k (a(0) = 1) where p_k is the k‑th
 *            prime. The permanent of the matrix M_{ij}=i*p_j is n! times the
 *            product of row entries times the product of column entries,
 *            yielding the above formula
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 36 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
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
    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* cache array of computed permanents   */
    static size_t cache_cap = 0;    /* capacity of cache in elements        */
    static idx_t max_n     = 0;     /* highest index that has been computed */
    static seq_t fact      = 1;     /* factorial of max_n                   */
    static seq_t prime_prod= 1;     /* product of the first max_n primes     */

    /* initialise cache lazily (needed even for n == 0) */
    if (cache_cap == 0) {
        cache_cap = 1;                     /* at least one element for a(0)       */
        cache = seq_alloc(cache_cap);      /* NULL on out‑of‑memory               */
        if (cache == NULL) return 0;       /* return 0 as error sentinel          */
        cache[0] = (seq_t)1;               /* a(0) = 1                            */
    }

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (tmp == NULL) return 0;                     /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n incrementally */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            fact *= (seq_t)i;                          /* factorial update   */
            prime_prod *= (seq_t)get_prime(i);         /* prime product update*/
            cache[i] = fact * fact * prime_prod;      /* permanent value    */
        }
        max_n = n;
    }

    return cache[n];
}

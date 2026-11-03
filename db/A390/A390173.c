/*
 * A390173  Smallest integer k with exactly n distinct prime factors such that
 *          the sum of those primes equals the sum of the exponents in its
 *          prime factorization.
 * https://oeis.org/A390173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * prime(n) * 2^(prime(n)-1), with a(1)=4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided utilities get_prime, pow2, seq_mul, seq_alloc_new_array, and seq_free_array. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/pow2.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cap     = 0;      /* allocated capacity */
static idx_t  length  = 0;      /* number of terms already computed */

/* Return the n-th term of the sequence, n>=1.
   For n==0 the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Ensure the cache can hold up to n terms. */
    if (n > cap) {
        idx_t new_cap = n * 2;                       /* oversize to reduce reallocs */
        seq_t *new_cache = seq_alloc_new_array((size_t)new_cap);

        if (cache) {
            /* Copy existing data to the new array. */
            for (idx_t i = 0; i < length; ++i)
                new_cache[i] = cache[i];
            seq_free_array(cache);                  /* free old array */
        }
        cache = new_cache;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = length + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)4;                    /* a(1) = 4 */
        } else {
            uint32_t p = get_prime(i);              /* i-th prime */
            seq_t   prime   = (seq_t)p;
            seq_t   two_pow = pow2((idx_t)(p - 1));   /* 2^(prime-1) */

            seq_t temp = seq_mul(cache[i - 2], prime); /* a(i-1) * prime */
            cache[i - 1] = seq_mul(temp, two_pow);      /* × 2^(prime-1) */
        }
    }

    length = n;
    return cache[n - 1];
}

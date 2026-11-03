/*
 * A308731  a(n) is the sum of the terms of the symmetric square array defined
 *          by M(i,j) = prime(i)+i-j for i >= j and M(i,j) = M(j,i) if i < j.
 * https://oeis.org/A308731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2
   a(n) = a(n-1) + (2*n-1)*prime(n) + n*(n-1)   for n ≥ 2   */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
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

/* Static cache for previously computed terms of the sequence. */
static seq_t *seq_cache = NULL;   /* dynamically allocated array of seq_t */
static idx_t cache_cap = 0;       /* allocated capacity (≥ 1 after first init) */
static idx_t cache_len = 0;       /* number of terms stored (≥ 1 after first init) */

/* Ensure seq_cache can hold at least n elements. */
static void ensure_seq_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < n)
        new_cap *= 2;

    /* realloc must be cast when compiling as C. */
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp) {
        /* If allocation fails, terminate the program safely. */
        /* In a library this could be replaced with an errno-based error. */
        abort();
    }
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 – return 0 for safety */
        return 0;

    /* Initialise first term if needed. */
    if (cache_len == 0) {
        ensure_seq_capacity(1);
        seq_cache[0] = 2;      /* a(1) = 2 */
        cache_len   = 1;
    }

    if (n <= cache_len)   /* cached value already available */
        return seq_cache[n - 1];

    /* Grow cache to hold all terms up to n. */
    ensure_seq_capacity(n);

    /* Compute terms from cache_len+1 up to n using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t p_k   = (seq_t)get_prime(k);          /* prime(k)   */
        seq_t term  = ((seq_t)(2 * k - 1)) * p_k;    /* (2k-1)*prime(k) */
        term += ((seq_t)k) * ((seq_t)(k - 1));       /* + k*(k-1)   */
        seq_cache[k - 1] = seq_cache[k - 2] + term;   /* a(k) = a(k-1)+term */
    }

    cache_len = n;
    return seq_cache[n - 1];
}

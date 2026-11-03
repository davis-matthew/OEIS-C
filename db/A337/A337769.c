/*
 * A337769  Smallest integer m such that the sum of the first m prime numbers
 *          is greater than n^2.
 * https://oeis.org/A337769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{m : Sum_{i=1..m} prime(i) > n^2}
 * Range:     u8 n <= 15, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
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
    /* target = n^2, computed in seq_t to avoid overflow */
    seq_t target = (seq_t)n * (seq_t)n;

    /* static state for caching prime sums */
    static seq_t *prefix = NULL;          /* array of cumulative sums */
    static size_t prefix_cap   = 0;       /* allocated capacity of prefix */
    static size_t prefix_len   = 0;       /* current number of stored sums */
    static size_t next_k       = 1;       /* next prime index to fetch (1‑based) */
    static seq_t cumulative_sum = 0;      /* current sum of primes */

    /* initialise cache on first call */
    if (prefix == NULL) {
        prefix_cap = 16;
        prefix = mem_new(seq_t, prefix_cap);
    }

    /* keep adding primes until the sum exceeds target */
    while (cumulative_sum <= target) {
        seq_t p = (seq_t)get_prime(next_k); /* get the next prime */
        cumulative_sum += p;

        /* grow the array if needed */
        if (prefix_len == prefix_cap) {
            prefix_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, prefix, prefix_cap);
            if (tmp) prefix = tmp; /* realloc handles failure silently for this use */
        }

        prefix[prefix_len++] = cumulative_sum;
        ++next_k;
    }

    /* next_k has been incremented past the prime that made the sum exceed target;
       the required m is the number of primes added so far, i.e. next_k‑1. */
    return (seq_t)(next_k - 1);
}

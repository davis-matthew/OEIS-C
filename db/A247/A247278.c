/*
 * A247278  Least integer k > 0 such that k*n - prime(k) is a square.
 * https://oeis.org/A247278
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 9003, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9582 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247278:
   a(n) = the least integer k > 0 such that k*n - prime(k) is a square.  */

/* The function implements the definition directly.  It sequentially
   tests k=1,2,... and stops when the first k satisfies the condition.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. */
    if (n < 2)
        return (seq_t)0;

    /* Search for the minimal k > 0 such that k*n - prime(k) is a square. */
    for (idx_t k = 1; ; ++k)
    {
        idx_t kn = k * n;                 /* k * n, safe in idx_t */
        uint32_t pk32 = get_prime(k);      /* prime(k) as uint32_t */
        idx_t pk  = (idx_t)pk32;           /* cast to idx_t for comparison */

        /* Once prime(k) > k*n the difference would become negative,
           and for larger k it will stay negative, so we can stop. */
        if (pk > kn)
            break;

        idx_t diff = kn - pk;              /* non‑negative difference */

        seq_t diff_seq = (seq_t)diff;      /* cast to seq_t for sqrt */
        seq_t s = floor_sqrt(diff_seq);    /* integer square root */

        if (s * s == diff_seq)             /* perfect square? */
            return (seq_t)k;               /* found the minimal k */
    }

    /* According to the conjecture the value always exists for n ≥ 2,
       so this line should never be reached.  */
    return (seq_t)0;
}

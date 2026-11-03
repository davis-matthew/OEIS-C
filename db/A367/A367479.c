/*
 * A367479  a(n) is the number of steps required for prime(n) to reach 2 when
 *          iterating the following hailstone map: If P == 5 (mod 6), then P
 *          -> next_prime(P + ceiling(sqrt(P))), otherwise P ->
 *          previous_prime(ceiling(sqrt(P))); or a(n) = -1 if prime(n) never
 *          reaches 2.
 * https://oeis.org/A367479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of steps required for prime(n) to reach 2
   using the hailstone map:
      if P % 6 == 5  ->  P = next_prime(P + ceil(sqrt(P)))
      otherwise      ->  P = prev_prime( ceil(sqrt(P)) )
   The function implements this map iteratively. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

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

/* Find the smallest prime ≥ start (linear search). */
static inline uint64_t next_prime_uint64(uint64_t start)
{
    for (uint64_t p = start;; ++p) {
        if (is_prime(p)) return p;
    }
}

/* Find the largest prime ≤ start (linear search). */
static inline uint64_t prev_prime_uint64(uint64_t start)
{
    for (uint64_t p = start;; --p) {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* offset 1: a(1)=0 for prime 2 */
    if (n < 1) return 0;

    /* starting prime */
    uint64_t p = (uint64_t)get_prime(n);
    seq_t steps = 0;

    while (p != 2) {
        /* ceil(sqrt(p)) */
        seq_t fsq_seq = floor_sqrt((seq_t)p);
        uint64_t fsq = (uint64_t)fsq_seq;       /* floor sqrt */
        uint64_t ceil_sqrt = fsq;
        if (ceil_sqrt * ceil_sqrt < p) ++ceil_sqrt;

        if ((p % 6ull) == 5ull) {
            /* next_prime(P + ceil_sqrt) */
            p = next_prime_uint64(p + ceil_sqrt);
        } else {
            /* prev_prime(ceil_sqrt) */
            p = prev_prime_uint64(ceil_sqrt);
        }
        ++steps;
    }

    return steps;
}

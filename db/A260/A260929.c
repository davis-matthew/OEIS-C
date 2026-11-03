/*
 * A260929  Least positive integer k < prime(n) such that k + k' is a
 *          triangular number, where k' is the unique integer among
 *          1,...,prime(n)-1 with k*k' == 1 (mod prime(n)), or 0 if no such k
 *          exists.
 * https://oeis.org/A260929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8748 (2026-11-01)
 * Helpers:   ipow_i32, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A260929
   a(n) = the least positive integer k < prime(n) such that
          k + k' is a triangular number, where k' ≡ k⁻¹ (mod prime(n)).
   If no such k exists, a(n) = 0.
   Implementation uses the provided utilities: get_prime, mod_pow, floor_sqrt.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/powmod.h"

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

seq_t a(idx_t n) {
    /* Ensure the index is valid.  a(1) corresponds to prime(1) = 2. */
    if (n == 0) return 0;

    /* Obtain the n-th prime.  It fits in 32 bits for all reasonable n. */
    uint32_t p32 = get_prime(n);
    seq_t   prime = (seq_t)p32;

    /* Search for the smallest k < prime satisfying the condition. */
    for (uint64_t k = 1; k < (uint64_t)p32; ++k) {
        seq_t k_ts   = (seq_t)k;

        /* k' is the modular inverse of k modulo prime.           */
        seq_t inv = powmod(k_ts, (uint64_t)p32 - 2, prime);

        seq_t sum = k_ts + inv;                   /* k + k'   */
        seq_t D   = (seq_t)1 + (seq_t)8 * sum;    /* 1 + 8*sum */

        seq_t s = floor_sqrt(D);
        if (s * s == D && (s - 1) % (seq_t)2 == 0) {
            /* k + k' is triangular.  Return the smallest such k. */
            return k_ts;
        }
    }

    /* No suitable k found. */
    return (seq_t)0;
}

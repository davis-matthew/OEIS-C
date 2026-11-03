/*
 * A377356  a(n) = Product{i = 1..(n-1)} prime(i)^e_i, where prime(i)^e_i is
 *          the smallest power of prime(i) which exceeds prime(n).
 * https://oeis.org/A377356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{p<prime(n)} p^e_p , where e_p is the least integer
 *            with p^e_p > prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 184 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the utility functions:
   get_prime, seq_pow, seq_mul.  All functions work with the seq_t type. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

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

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 */

    /* Prime of the nth index (0‑based offset is 1). */
    uint32_t pn32 = get_prime(n);
    uint64_t prime_n = pn32;   /* use 64‑bit for comparison */

    seq_t result = (seq_t)1;

    for (idx_t k = 1; k < n; ++k) {
        uint32_t pk32 = get_prime(k);
        uint64_t pk = pk32;

        /* Find smallest exponent e such that pk^e > prime_n */
        uint64_t pow = pk;
        unsigned int e = 1;
        while (pow <= prime_n) {
            pow *= pk;
            ++e;
        }

        /* Multiply the contribution of this prime into the result. */
        seq_t power = ipow_idx((seq_t)pk32, (idx_t)e);
        result = seq_mul(result, power);
    }

    return result;
}

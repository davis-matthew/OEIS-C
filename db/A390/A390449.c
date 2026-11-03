/*
 * A390449  Heinz number of first sums of prime indices of n.
 * https://oeis.org/A390449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) is the Heinz number of the first sums of prime indices of n
 *            (definition as in OEIS A390449)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   build_primes_upto, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/prime_pi.h"

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
    if (n < 2)            /* n = 1 => empty multiset => result 1 */
        return (seq_t)1;

    /* Build a prime list up to n the first time or whenever n is larger */
    static uint64_t *prime_tbl = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    if (prime_tbl == NULL || n > prime_limit) {
        if (prime_tbl) free(prime_tbl);
        build_primes_upto(n, &prime_tbl, &prime_cnt);
        prime_limit = n;
    }

    uint64_t m = (uint64_t)n;          /* mutable copy for factorisation */
    seq_t result = (seq_t)1;
    bool have_prev = false;
    uint64_t prev_idx = 0;

    for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= m; ++i) {
        uint64_t p = prime_tbl[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t idx = prime_pi(p);            /* index of prime p */
            uint64_t idx64 = (uint64_t)idx;
            for (int k = 0; k < exp; ++k) {
                if (!have_prev) {
                    prev_idx = idx64;
                    have_prev = true;
                } else {
                    uint64_t sum = prev_idx + idx64;
                    seq_t prime_sum = get_prime(sum);
                    result = seq_mul(result, prime_sum);
                    prev_idx = idx64;
                }
            }
        }
    }

    /* Remaining prime factor greater than sqrt(n) */
    if (m > 1) {
        uint64_t p = m;
        seq_t idx = prime_pi(p);
        uint64_t idx64 = (uint64_t)idx;
        if (!have_prev) {
            prev_idx = idx64;
            have_prev = true;
        } else {
            uint64_t sum = prev_idx + idx64;
            seq_t prime_sum = get_prime(sum);
            result = seq_mul(result, prime_sum);
            prev_idx = idx64;
        }
    }

    return result;
}

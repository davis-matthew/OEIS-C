/*
 * A305048  Number of ordered pairs (k, m) of nonnegative integers such that
 *          5^k + 10^m is not only a primitive root modulo prime(n) but also
 *          smaller than prime(n).
 * https://oeis.org/A305048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2169, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 106 terms through n = 8888 (2026-11-01)
 * Helpers:   factor_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305048: For the n‑th prime p, count ordered pairs (k,m) of
 * non‑negative integers such that g = 5^k + 10^m satisfies
 * g < p and g is a primitive root modulo p. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
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

/* Return the number of admissible pairs for the n‑th prime. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* n‑th prime (1‑based indexing) */
    seq_t p = get_prime(n);

    /* p-1 is small enough for 64‑bit arithmetic (valid for the
     * intended range n ≤ 10 000). */
    uint64_t p_minus1 = (uint64_t)p - 1;

    /* Factor p-1 into distinct prime factors. */
    uint64_t prime_factors[64], exponents[64];
    size_t factor_cnt = 0;
    factor_u64(p_minus1, prime_factors, exponents, &factor_cnt);

    seq_t count = 0;
    seq_t pow5 = 1;        /* current 5^k */

    while (pow5 < p) {
        seq_t pow10 = 1;  /* current 10^m */
        while (pow5 + pow10 < p) {
            seq_t g = pow5 + pow10;

            /* Check if g is a primitive root modulo p. */
            bool ok = true;
            for (size_t i = 0; i < factor_cnt && ok; ++i) {
                uint64_t q   = prime_factors[i];
                uint64_t exp = p_minus1 / q;
                seq_t     res = powmod(g, exp, p);
                if (res == (seq_t)1) ok = false;
            }
            if (ok) ++count;

            pow10 = pow10 * (seq_t)10;
        }
        pow5 = pow5 * (seq_t)5;
    }

    return count;
}

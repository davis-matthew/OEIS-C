/*
 * A286571  Compound filter (prime signature of n & n/gcd(n, sigma(n))): a(n)
 *          = P(A046523(n), A017666(n)), where P(n,k) is sequence A000027 used
 *          as a pairing function.
 * https://oeis.org/A286571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A046523(n)+A017666(n))^2) - A046523(n) -
 *            3*A017666(n)) where A046523(n) is the prime signature mapped to
 *            the first primes (exponents sorted in non‑increasing order) and
 *            A017666(n) = n / gcd(n, sigma(n))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9591 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Compute A046523(n): prime signature mapped to first primes.
       Extract prime exponents, sort them descending, then multiply
       the first primes raised to those exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Sort exponents in non‑increasing order. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[j] > exps[i]) {
                uint64_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
        }
    }

    seq_t m = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)get_prime((idx_t)(i + 1));          /* i‑th first prime (2,3,5,…) */
        seq_t term = ipow_idx(p, (idx_t)exps[i]);            /* p^e */
        m = seq_mul(m, term);                              /* accumulate product */
    }

    /* Compute A017666(n): n / gcd(n, sigma(n)). */
    seq_t sn = (seq_t)n;
    seq_t sigma = sigma_of(n);
    seq_t g = gcd(sn, sigma);
    seq_t p = sn / g;

    /* Apply the closed formula. */
    seq_t sum = m + p;
    seq_t sum_sq = sum * sum;
    seq_t t = sum_sq - m - seq_mul(p, (seq_t)3) + (seq_t)2;
    seq_t ans = t / (seq_t)2;

    return ans;
}

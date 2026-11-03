/*
 * A294874  a(n) = Product_{d|n, d>1, d = x^(2k) for some maximal k} prime(k).
 * https://oeis.org/A294874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 575, u16 n <= 9215, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9154 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Product_{d|n, d>1, d = x^(2k) for some maximal k} prime(k)
 *
 * This implementation factors n, computes for each divisor d>1 that is a perfect
 * square power the maximal exponent k = min_i (v_i / 2) where v_i are the
 * prime exponents of d.  It counts how many divisors have a given k and
 * multiplies the k‑th prime that many times.  The algorithm runs in time
 * proportional to the number of distinct prime factors of n and the
 * largest exponent in n, well below the required limits for n ≤ 10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

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

/* ------------------------------------------------------------------
 * Computes the OEIS sequence A294874 for index n (1‑based).
 * ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* n = 1 has no divisors > 1 */
    if (pf_cnt == 0) {
        return (seq_t)1;
    }

    /* Compute e'_i = floor(e_i / 2) for each prime factor */
    uint64_t eprime[64];
    uint64_t max_eprime = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        eprime[i] = exps[i] / 2;
        if (eprime[i] > max_eprime) {
            max_eprime = eprime[i];
        }
    }

    seq_t result = (seq_t)1;   /* cumulative product */

    /* For each possible minimal t = k (≥ 1) */
    for (uint64_t m = 1; m <= max_eprime; ++m) {
        /* Count divisors with minimal exponent = m */
        seq_t prod1 = (seq_t)1;  /* sum over assignments where each chosen prime exponent ≥ m */
        seq_t prod2 = (seq_t)1;  /* sum over assignments where each chosen prime exponent > m                */
        for (size_t i = 0; i < pf_cnt; ++i) {
            if (eprime[i] >= m) {
                seq_t val1 = (seq_t)(eprime[i] - m + 2); /* 1 (exp=0) + (e'_i - m + 1) options ≥ m */
                seq_t val2 = (seq_t)(eprime[i] - m + 1); /* 1 (exp=0) + (e'_i - m)   options > m */
                prod1 = seq_mul(prod1, val1);
                prod2 = seq_mul(prod2, val2);
            } /* else both remain unchanged (multiplied by 1) */
        }
        seq_t cnt_m = prod1 - prod2;   /* number of divisors with minimal t = m */

        if (cnt_m == 0) {
            continue;
        }

        /* Multiply the product by prime(m) raised to cnt_m */
        seq_t prime_val = (seq_t)get_prime(m);
        seq_t power_term = ipow(prime_val, cnt_m);
        result = seq_mul(result, power_term);
    }

    return result;
}

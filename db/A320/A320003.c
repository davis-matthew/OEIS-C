/*
 * A320003  Number of proper divisors of n of the form 6*k + 3.
 * https://oeis.org/A320003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of proper divisors of n that are odd multiples of 3
 *            (i.e., d | n, d < n, d % 6 == 3). The function enumerates all
 *            divisors of n using its prime factorisation and counts those
 *            that satisfy the congruence. It uses the helper functions
 *            build_primes() and floor_sqrt() from the utils library
 * Range:     u8 n <= 63, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9985 (2026-11-02)
 * Helpers:   build_primes, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes.h"
#include "../utils/ipow_i32.h"

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

static idx_t prime_limit = 0;            /* largest limit already built */
static idx_t *prime_cache = NULL;        /* array of primes up to prime_limit */
static size_t prime_cnt = 0;             /* number of primes in prime_cache */

/* Ensure prime_cache contains all primes <= limit */
static void ensure_primes(idx_t limit) {
    if (limit <= prime_limit) return;
    free(prime_cache);                   /* free old array if any */
    build_primes(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

/* Return the number of proper divisors of n that are odd multiples of 3. */
seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* Step 1: factorise n by trial division using small primes. */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);
    ensure_primes(sqrt_n);

    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint8_t exps[64];
    size_t pcnt = 0;

    for (size_t i = 0; i < prime_cnt && prime_cache[i] * prime_cache[i] <= tmp; ++i) {
        uint64_t p = (uint64_t)prime_cache[i];
        if (tmp % p != 0) continue;
        uint8_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt]   = e;
        ++pcnt;
    }
    if (tmp > 1) {           /* remaining prime factor */
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Step 2: generate all divisors of n.  The total number of divisors
       is bounded by the product of (exps[i]+1), which is small for n <= 10000. */
    uint64_t divs[4096];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t j = 0; j < pcnt; ++j) {
        size_t cur = dcnt;
        uint64_t p = primes[j];
        uint64_t pk = 1;
        for (uint8_t k = 1; k <= exps[j]; ++k) {
            pk *= p;
            for (size_t i = 0; i < cur; ++i) {
                divs[dcnt++] = divs[i] * pk;
            }
        }
    }

    /* Step 3: count proper divisors that are odd multiples of 3. */
    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == n) continue;          /* proper divisor only */
        if (d % 6 == 3) ++count;
    }

    return count;
}

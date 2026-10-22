/*
 * A349484  Niven numbers whose arithmetic derivative is also a Niven number
 *          (A005349).
 * https://oeis.org/A349484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth smallest integer m>0 such that * m is a Niven
 *            number and its arithmetic derivative m' is also a Niven number
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/digit_sum.h"

/* --- local helpers ----------------------------------------------------- */

static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = ~((uint64_t)0) >> 1;          /* sqrt(2^64-1) <= 2^32-1 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* test whether a 64‑bit unsigned integer is a Niven number */
static bool isNiven_uint64(uint64_t v) {
    seq_t vsum = digit_sum((seq_t)v);
    if (vsum == (seq_t)0) return false;
    return ((seq_t)v % vsum) == (seq_t)0;
}

/* compute the arithmetic derivative of a 64‑bit integer */
static seq_t derivative_uint64(uint64_t m);

/* ---------------------------------------------------------------------- */

/* --- global cache of sequence terms ----------------------------------- */
static seq_t *term_cache      = NULL;
static size_t term_cnt         = 0;
static size_t term_cap         = 0;

/* --- prime table used for factorisation ----------------------------- */
static uint64_t *prime_list    = NULL;
static size_t   prime_cnt      = 0;
static size_t   prime_cap      = 0;
static uint64_t prime_last     = 0;   /* largest number we have inspected */

/* current integer last tested in the global search */
static idx_t    last_checked   = 0;

/* ---------------------------------------------------------------------- */

static void grow_terms(size_t min_cap) {
    if (term_cap >= min_cap) return;
    if (term_cap == 0) term_cap = 64;
    while (term_cap < min_cap) term_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, term_cap * sizeof(seq_t));
    assert(term_cache != NULL);
}

static void grow_primes(size_t min_cap) {
    if (prime_cap >= min_cap) return;
    if (prime_cap == 0) prime_cap = 64;
    while (prime_cap < min_cap) prime_cap <<= 1;
    prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    assert(prime_list != NULL);
}

/* extend the known prime list to include all primes up to n (inclusive) */
static void extend_primes_to(uint64_t n) {
    if (prime_last >= n) return;
    uint64_t start = (prime_last <= 2) ? 2 : prime_last + 1;
    if (start <= 2) {
        if (prime_cnt == 0) {
            grow_primes(1);
            prime_list[0] = 2;
            prime_cnt = 1;
        }
        start = 3;
    }
    for (uint64_t cand = start; cand <= n; cand += (cand == 2 ? 1 : 2)) {
        bool isprime = true;
        uint64_t limit = isqrt_u64(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= limit; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            grow_primes(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;
        }
    }
    prime_last = n;
}

/* compute the arithmetic derivative of a 64‑bit integer */
static seq_t derivative_uint64(uint64_t m) {
    uint64_t original = m;
    uint64_t temp = m;
    seq_t der = (seq_t)0;

    extend_primes_to(isqrt_u64(original));

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if ((__uint128_t)p * p > temp) break;
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            der += ((seq_t)(original / p)) * e;
        }
    }
    if (temp > 1) {
        der += (seq_t)(original / temp);   /* remaining prime factor, exponent 1 */
    }
    return der;
}

static void append_term(seq_t val) {
    grow_terms(term_cnt + 1);
    term_cache[term_cnt++] = val;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n) {
    assert(n > 0);

    /* grow cache until we have at least n terms */
    while (term_cnt < n) {
        last_checked++;
        uint64_t cand = (uint64_t)last_checked;

        if (!isNiven_uint64(cand))
            continue;

        seq_t der = derivative_uint64(cand);
        if (isNiven_uint64((uint64_t)der)) {
            append_term((seq_t)cand);
        }
    }

    return term_cache[n - 1];
}

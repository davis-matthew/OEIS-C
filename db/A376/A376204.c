/*
 * A376204  Numbers whose sum of powerful divisors (including 1) is a powerful
 *          number that is larger than 1.
 * https://oeis.org/A376204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th positive integer k such that the sum of its *
 *            powerful divisors (1 included) is itself a powerful number >1. *
 *            The implementation enumerates integers k, computes * sum_{d|k, d
 *            powerful or d==1} d as a product of * (1 + Σ_{j=2..e} p^j) over
 *            the primes p^e | k, * then tests whether...
 * Range:     u8 n <= 2, u16 n <= 614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static const idx_t MAX_LIMIT = 5000000;          /* sieve range for factorisation */
static uint32_t *spf = NULL;                    /* smallest prime factor array   */
static uint32_t *primes = NULL;                  /* list of primes up to MAX_LIMIT*/
static size_t primes_cnt = 0;                    /* number of primes in list      */

static idx_t cur_candidate = 1;                  /* last tested integer          */
static seq_t *cached = NULL;                     /* array of qualifying numbers   */
static size_t cached_fit = 0;                    /* allocated capacity           */
static size_t cached_len  = 0;                    /* number of stored terms       */
static bool  initialized = false;                /* one‑time initialisation      */

/* Allocate the SPF array and generate all primes up to MAX_LIMIT.  */
static void init_sieve(void)
{
    spf = (uint32_t *)malloc(MAX_LIMIT * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i < MAX_LIMIT; ++i) spf[i] = i;

    for (uint32_t i = 2; (uint64_t)i * i < MAX_LIMIT; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j < MAX_LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;

    /* Build prime list for trial division of numbers > MAX_LIMIT. */
    primes_cnt = 0;
    primes = (uint32_t *)malloc(MAX_LIMIT / 10 * sizeof(uint32_t)); /* rough size */
    for (uint32_t i = 2; i < MAX_LIMIT; ++i)
        if (spf[i] == i) primes[primes_cnt++] = i;
}

/* Return product over prime factors of k of the sum of allowed powers.
 * k is factored using spf.  */
static seq_t sum_powerful_divs(uint32_t k)
{
    seq_t total = 1;                       /* start with divisor 1          */
    uint32_t tmp = k;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        int exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }

        /* Compute 1 + Σ_{j=2..exp} p^j  */
        seq_t term = 1;                    /* exponent 0 */
        if (exp >= 2) {
            seq_t ppow = (seq_t)p * p;     /* p^2          */
            term += ppow;                  /* j=2          */
            for (int j = 3; j <= exp; ++j) {
                ppow *= p;
                term += ppow;
            }
        }
        total *= term;
    }
    return total;
}

/* Test whether n (>1) is a powerful number.
 * For n <= MAX_LIMIT use spf, otherwise trial division by primes. */
static bool is_powerful(uint64_t n)
{
    if (n <= 1) return false;
    if (n < MAX_LIMIT) {
        while (n > 1) {
            uint32_t p = spf[(uint32_t)n];
            int exp = 0;
            while (n % p == 0) { n /= p; ++exp; }
            if (exp == 1) return false;
        }
        return true;
    }
    /* trial division using precomputed small primes */
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        int exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }
        if (exp == 1) return false;
    }
    /* Remaining factor > 1 is prime with exponent 1 */
    return tmp <= 1;
}

/* Ensure cached array has room for at least new_len elements. */
static void ensure_cache_capacity(size_t new_len)
{
    if (new_len <= cached_fit) return;
    size_t new_cap = cached_fit ? cached_fit * 2 : 64;
    if (new_cap < new_len) new_cap = new_len;
    cached = (seq_t *)realloc(cached, new_cap * sizeof(seq_t));
    if (!cached) exit(EXIT_FAILURE);
    cached_fit = new_cap;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        cached = (seq_t *)malloc(64 * sizeof(seq_t));
        cached_fit = 64;
        cached_len = 0;
        initialized = true;
    }

    while (cached_len < (size_t)n) {
        ++cur_candidate;
        seq_t sum = sum_powerful_divs((uint32_t)cur_candidate);
        if (sum > 1 && is_powerful((uint64_t)sum)) {
            ensure_cache_capacity(cached_len + 1);
            cached[cached_len++] = (seq_t)cur_candidate;
        }
    }
    return cached[n - 1];
}

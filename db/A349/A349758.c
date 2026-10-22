/*
 * A349758  Nobly abundant numbers: numbers k such that both d(k) = A000005(k)
 *          and sigma(k) = A000203(k) are abundant numbers (A005101).
 * https://oeis.org/A349758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate through integers k and select those for which
   both the divisor count d(k) and the sum of divisors σ(k) are
   abundant numbers (σ(n) > 2n).  The sequence is generated lazily
   and cached so that any call to a(n) is answered in O(1) after the
   necessary terms have been computed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Prime sieve storage */
static uint64_t *primes = NULL;   /* array of primes */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t primes_cap = 0;     /* capacity of prime array */
static uint64_t prime_limit = 0;  /* largest number for which primes are known */

/* Cache for sequence values */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_len = 0;      /* number of terms cached */
static size_t cache_cap = 0;      /* capacity of cache array */

/* ------------------------------------------------------------------ */
/* Utility: integer square root (uint64_t) using double approximation   */
static uint64_t isqrt_u64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t r = (uint64_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* Ensure we have all primes up to 'limit'.  Simple sieve; the limit
   is assumed small enough to fit in memory. */
static void ensure_primes_up_to(uint64_t limit) {
    if (limit <= prime_limit) return;
    size_t sz = (size_t)limit + 1;
    unsigned char *sieve = (unsigned char*)calloc(sz, 1);
    if (!sieve) abort();
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!sieve[p]) {
            for (uint64_t j = p * p; j <= limit; j += p)
                sieve[j] = 1;
        }
    }
    /* Count primes and allocate */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;
    uint64_t *newprimes = (uint64_t*)realloc(primes, cnt * sizeof(uint64_t));
    if (!newprimes) abort();
    primes = newprimes;
    primes_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) primes[primes_cnt++] = i;
    prime_limit = limit;
    free(sieve);
}

/* ------------------------------------------------------------------ */
/* Compute divisor count τ(n) and sum of divisors σ(n) for a uint64_t
   n.  The results are returned via pointers tau and sigma (both
   uint64_t).  Only the primes up to sqrt(n) are required. */
static void compute_tau_sigma_uint64(uint64_t n,
                                     uint64_t *tau,
                                     uint64_t *sigma) {
    uint64_t temp = n;
    uint64_t d = 1;
    uint64_t s = 1;
    ensure_primes_up_to(isqrt_u64(temp) + 1);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            uint64_t power = 1;
            uint64_t term = 1;        /* sum of p^0 .. p^exp */
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                power *= p;
                term += power;
            }
            d *= (exp + 1);
            s *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        d *= 2;
        s *= (temp + 1);
    }
    *tau = d;
    *sigma = s;
}

/* Return σ(n) for a uint64_t n (used in abundance test). */
static uint64_t sigma_of_uint64(uint64_t n) {
    uint64_t dummy;
    uint64_t sigma;
    compute_tau_sigma_uint64(n, &dummy, &sigma);
    return sigma;
}

/* Test if n is abundant: σ(n) > 2n */
static bool is_abundant_uint64(uint64_t n) {
    uint64_t sig = sigma_of_uint64(n);
    return sig > 2 * n;
}

/* Ensure that the cache has capacity for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* The required OEIS sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for 0, return 0 for safety */

    static idx_t next_candidate = 1;  /* next integer to test */

    /* Grow cache until we have n terms */
    while (cache_len < (size_t)n) {
        uint64_t tau, sigma_k;
        compute_tau_sigma_uint64((uint64_t)next_candidate, &tau, &sigma_k);

        bool abundant_tau   = is_abundant_uint64(tau);
        bool abundant_sigma = is_abundant_uint64((uint64_t)sigma_k);

        if (abundant_tau && abundant_sigma) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)next_candidate;
            ++cache_len;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

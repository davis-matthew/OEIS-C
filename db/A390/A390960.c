/*
 * A390960  The number of noninfinitary divisors of the numbers whose number
 *          of divisors is not a power of 2 (A162643).
 * https://oeis.org/A390960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A348341(A162643(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A162643(n) is the n-th positive integer whose divisor
    count τ is not a power of two, and
    A348341(m) is the number of non‑infinitary divisors of m.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Maximum integer to sieve for smallest prime factor.   */
#define MAX_SIEVE 400000U

/* Smallest prime factor table.  */
static uint32_t spf[MAX_SIEVE + 1];
static bool spf_built = false;

/* Build spf table up to MAX_SIEVE.  */
static void build_spf(void) {
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) spf[i] = 0;
    spf[1] = 1;
    for (uint32_t i = 2; i * i <= MAX_SIEVE; ++i) {
        if (!spf[i]) {  /* i is prime */
            spf[i] = i;
            for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i)
        if (!spf[i]) spf[i] = i;
    spf_built = true;
}

/* Factor a number, returning tau (divisor count) and sum of
   binary popcounts of the prime exponents.   */
static void factor_tau_bits(uint64_t val, uint64_t *tau_out, unsigned *bits_out) {
    uint64_t tau = 1;
    unsigned bits = 0;
    while (val > 1) {
        uint32_t p = spf[val];
        unsigned exp = 0;
        while (val % p == 0) {
            val /= p;
            ++exp;
        }
        tau *= (exp + 1);
        bits += __builtin_popcount(exp);
    }
    *tau_out = tau;
    *bits_out = bits;
}

/* Cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size (in seq_t units)       */
static size_t cache_used = 0;     /* number of terms stored (incl. dummy 0)*/
static idx_t next_candidate = 0;  /* next integer to test                 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but avoid UB */

    /* Build smallest prime factor table once. */
    if (!spf_built) build_spf();

    /* Ensure cache has size at least n+1 (index 0 unused). */
    if (cache_used < (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > cache_cap) {
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute additional terms up to n. */
        for (size_t idx = cache_used + 1; idx <= (size_t)n; ++idx) {
            /* Find next integer whose divisor count is NOT a power of two. */
            while (true) {
                ++next_candidate;
                if (next_candidate > MAX_SIEVE) {
                    /* Extremely unlikely for n <= 10000, but guard anyway. */
                    /* Build a bigger spf table if needed. */
                    exit(EXIT_FAILURE);
                }
                uint64_t tau;
                unsigned bits;
                factor_tau_bits((uint64_t)next_candidate, &tau, &bits);
                if ((tau & (tau - 1)) == 0) continue;  /* tau is a power of two */
                /* Compute number of noninfinitary divisors: tau - 2^bits */
                seq_t infinitary = (seq_t)(1ULL << bits);
                seq_t result = (seq_t)tau - infinitary;
                cache[idx] = result;
                break;
            }
        }
        cache_used = (size_t)n;
    }
    return cache[(size_t)n];
}

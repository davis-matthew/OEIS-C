/*
 * A354945  a(n) is the least k > 0 such that n is a non-isolated divisor of
 *          k!.
 * https://oeis.org/A354945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max( A002034(n), min( A002034(n-1), A002034(n+1) ) ),
 *            with a(1) = 2. A002034(n) is the least k such that n | k!
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   min_k_for_p_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/min_k_for_p_exp.h"

/* Cache for the A002034 values */
static seq_t *f_cache = NULL;
static idx_t f_cache_alloc = 0;

/* Sieve data for smallest prime factor */
static uint64_t *spf = NULL;
static idx_t spf_size = 0;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure the spf array is computed up to at least `limit`. */
static void ensure_spf(idx_t limit) {
    if (spf_size > limit) return;
    idx_t nlimit = limit + 1;

    spf     = (uint64_t *)realloc(spf,     nlimit * sizeof(uint64_t));
    primes  = (uint64_t *)realloc(primes,  nlimit * sizeof(uint64_t));

    for (idx_t i = 0; i < nlimit; ++i) spf[i] = 0;
    primes_cnt = 0;
    for (idx_t i = 2; i < nlimit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p > spf[i]) break;
            uint64_t v = i * p;
            if (v > limit) break;
            spf[v] = p;
        }
    }
    spf_size = nlimit;
}

/* Ensure the f_cache array can hold values up to index `n`. */
static void ensure_f_cache(idx_t n) {
    if (n < f_cache_alloc) return;
    idx_t new_size = (n + 1) * 2; /* grow faster to reduce reallocs */
    f_cache = (seq_t *)realloc(f_cache, new_size * sizeof(seq_t));
    for (idx_t i = f_cache_alloc; i < new_size; ++i) f_cache[i] = 0;
    f_cache_alloc = new_size;
}

/* Compute A002034(n) – the minimal k such that n | k!. */
static seq_t compute_f(idx_t n) {
    if (n <= 1) return 1;
    ensure_spf(n);
    idx_t temp = n;
    seq_t result = 1;
    while (temp > 1) {
        uint64_t p = spf[temp];
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        seq_t k = min_k_for_p_exp(p, e, n);
        if (k > result) result = k;
    }
    return result;
}

/* Memoised getter for A002034 values. */
static seq_t get_f(idx_t n) {
    ensure_f_cache(n);
    if (f_cache[n] == 0) f_cache[n] = compute_f(n);
    return f_cache[n];
}

/* Public sequence function.  All arguments are 1‑based. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    seq_t f_n   = get_f(n);
    seq_t f_nm1 = get_f(n - 1);
    seq_t f_np1 = get_f(n + 1);
    seq_t m = f_nm1 < f_np1 ? f_nm1 : f_np1;
    return f_n > m ? f_n : m;
}

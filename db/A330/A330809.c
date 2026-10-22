/*
 * A330809  Triangular numbers having exactly 8 divisors.
 * https://oeis.org/A330809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth triangular number having exactly 8 divisors (search)
 * Range:     u8 n <= 6, u16 n <= 77, u32 n <= 5499, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------- dynamic cache for already found terms ------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t max_k     = 0;

/* ------- pre‑computed smallest prime factor table ------- */
static const idx_t SPF_LIMIT = 2000000;          /* enough for k up to ~1e6   */
static uint32_t   *spf       = NULL;
static idx_t        spf_limit = 0;

/* ---------- initialise spf table once ---------- */
static void init_spf(void) {
    if (spf != NULL) return;
    spf_limit = SPF_LIMIT;
    spf = (uint32_t *)malloc((spf_limit + 1) * sizeof(uint32_t));
    if (!spf) return;                         /* unlikely, out of memory */
    for (idx_t i = 0; i <= spf_limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((idx_t)i * (idx_t)i <= spf_limit) {
                for (idx_t j = i * i; j <= spf_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ---------- factorisation product of (exp+1) ---------- */
static inline uint64_t factor_product(idx_t n) {
    uint64_t prod = 1;
    while (n > 1) {
        uint32_t p = (n <= spf_limit ? spf[n] : (uint32_t)n);
        uint64_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        prod *= (exp + 1);
    }
    return prod;
}

/* ---------- divisor count of k(k+1)/2 ---------- */
static inline uint64_t divisor_count_triangular(idx_t k) {
    idx_t a = k, b = k + 1;
    if (a & 1) b >>= 1; else a >>= 1;
    uint64_t d_a = factor_product(a);
    uint64_t d_b = factor_product(b);
    return d_a * d_b;
}

/* ---------- append new term to cache ---------- */
static inline void append_value(seq_t val) {
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return;                      /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- exported function a(n) ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* undefined for offset 0 */
    if (cache_len >= (size_t)n) return cache[n - 1];
    init_spf();
    while (cache_len < (size_t)n) {
        ++max_k;
        if (divisor_count_triangular(max_k) == 8) {
            seq_t tri = (seq_t)max_k * ((seq_t)max_k + 1) / 2;
            append_value(tri);
        }
    }
    return cache[n - 1];
}

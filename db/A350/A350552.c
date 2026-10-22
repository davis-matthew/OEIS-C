/*
 * A350552  Numbers k for which A000120(k) = A000120(k'), where k' is
 *          arithmetic derivative of k (A003415).
 * https://oeis.org/A350552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A350552: numbers k for which popcount(k) equals popcount(k′),
   where k′ is the arithmetic derivative of k.  The routine
   enumerates integers in increasing order, testing the condition
   and caching the results.  It is O(n) with a small constant factor. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Build smallest prime factor table (provided by the utilities). */
#include "../utils/build_spf.h"

static inline unsigned popcount_seq_t(seq_t x) {
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
}

/* Ensure the SPF table covers all numbers up to 'limit'. */
static idx_t *spf_tbl = NULL;
static idx_t spf_limit = 0;
static void ensure_spf(idx_t limit) {
    if (limit <= spf_limit) return;
    free(spf_tbl);
    build_spf(limit, &spf_tbl);   /* builds array 1..limit */
    spf_limit = limit;
}

/* Arithmetic derivative: k' = k * Σ(eᵢ / pᵢ).  k is an idx_t value. */
static inline seq_t derivative_of(seq_t k) {
    if (k <= 1) return 0;
    uint64_t result = 0;
    seq_t   orig   = k;
    idx_t   n      = (idx_t)k;
    while (n > 1) {
        idx_t p   = spf_tbl[n];
        uint64_t exp = 0;
        do { exp++; n /= p; } while (n % p == 0);
        uint64_t div = (uint64_t)(orig / p);
        result += exp * div;
    }
    return (seq_t)result;
}

/* Cache of found terms. */
static seq_t *cache     = NULL;
static size_t cache_sz  = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* capacity of the cache array */
static idx_t  max_checked = 0; /* largest integer examined so far */

/* Append value to cache, reallocating if necessary. */
static void cache_append(seq_t val) {
    if (cache_sz == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { free(cache); exit(1); }
        cache     = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* Sequence a(n), n ≥ 1.  Negligible overhead for repeated calls. */
seq_t a(idx_t n) {
    /* Lazy initialisation of SPF table. */
    if (!spf_tbl && !spf_limit) {
        ensure_spf(2000000);
    }

    /* First term (k = 0). */
    if (cache_sz == 0) {
        cache_append((seq_t)0);
        max_checked = 0;
    }

    /* Generate terms until we have the required n-th term. */
    while ((idx_t)cache_sz < n) {
        idx_t next = max_checked + 1;
        ++max_checked;

        seq_t k = (seq_t)next;
        bool good = (k == 0);          /* 0 is always a term */
        if (!good) {
            unsigned pc_k = popcount_seq_t(k);
            seq_t deriv   = derivative_of(k);
            unsigned pc_d = popcount_seq_t(deriv);
            good = (pc_k == pc_d);
        }
        if (good) cache_append(k);

        /* Expand SPF table if needed. */
        if (next > spf_limit) {
            idx_t new_limit = spf_limit * 2 + 1000;
            if (new_limit < next) new_limit = next * 2;  /* safety */
            ensure_spf(new_limit);
        }
    }

    return cache[n - 1];
}

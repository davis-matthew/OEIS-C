/*
 * A360191  G.f. 1 / Product_{n>=1} (1 - x^n)^3 * (1 - x^(2*n-1))^2.
 * https://oeis.org/A360191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 35, u64 n <= 112, u128 n <= 383,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  A(x) = 1 / Prod_{n≥1} (1−x^n)^3 (1−x^{2n−1})^2
 * This is equivalent to a(0)=1 and for n≥1:
 *   a(n) = - Σ_{k=1..n} b(k) * a(n−k)
 * where b(k) = 6m+1 if k=m(3m+1)/2
 *        b(k) = −6m+1 if k=m(3m−1)/2
 *        b(k) = 0 otherwise (m≥1).
 * The implementation stores the necessary pentagonal numbers and b(k)
 * coefficients in static arrays and memoizes the sequence values
 * in a growing cache.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- pentagonal data ----------
 * pent_pos holds the pentagonal indices k in ascending order.
 * pent_val holds the corresponding coefficient b(k).
 * pent_len is the number of valid entries.
 * pent_cap is the allocated capacity. ----------
 */
static idx_t  *pent_pos = NULL;
static int16_t *pent_val = NULL;
static size_t  pent_len = 0;   /* used entries */
static size_t  pent_cap = 0;   /* allocated entries */

/* Resize pentagonal arrays when needed. */
static void ensure_pent_cap(size_t cap) {
    if (pent_cap < cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 16;
        idx_t  *new_pos = (idx_t *)realloc(pent_pos, new_cap * sizeof(idx_t));
        int16_t *new_val = (int16_t *)realloc(pent_val, new_cap * sizeof(int16_t));
        if (!new_pos || !new_val) {
            /* allocation failure – abort cleanly */
            abort();
        }
        pent_pos = new_pos;
        pent_val = new_val;
        pent_cap = new_cap;
    }
}

/* Build pentagonal numbers and coefficients up to the given limit
 * (i.e., produce all k <= limit-1). */
static void build_pentaginals(size_t limit) {
    /* Estimate number of pentagonal numbers needed. */
    size_t m, count = 0;
    for (m = 1; ; ++m) {
        unsigned __int128 p1 = (unsigned __int128)m * (3 * m + 1) / 2;
        unsigned __int128 p2 = (unsigned __int128)m * (3 * m - 1) / 2;
        if (p1 > limit - 1 && p2 > limit - 1) break;
        if (p2 <= limit - 1) ++count;
        if (p1 <= limit - 1) ++count;
    }
    ensure_pent_cap(count);
    pent_len = 0;
    for (m = 1; ; ++m) {
        unsigned __int128 p2 = (unsigned __int128)m * (3 * m - 1) / 2;
        unsigned __int128 p1 = (unsigned __int128)m * (3 * m + 1) / 2;
        if (p1 > limit - 1 && p2 > limit - 1) break;
        if (p2 <= limit - 1) {
            pent_pos[pent_len] = (idx_t)p2;
            pent_val[pent_len] = -(6 * m) + 1;
            ++pent_len;
        }
        if (p1 <= limit - 1) {
            pent_pos[pent_len] = (idx_t)p1;
            pent_val[pent_len] =  6 * m + 1;
            ++pent_len;
        }
    }
}

/* ---------- sequence cache ----------
 * cache holds the memoised values a(n).
 * max_computed is the largest index already filled. ----------
 */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t max_computed = 0;

/* Expand cache and fled compute until n is ready. */
static void compute_up_to(idx_t n) {
    /* Extend cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
        /* Rebuild pentagon tables for the new maximum. */
        build_pentaginals(cache_cap);
    }

    /* Compute new values. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < pent_len && pent_pos[j] <= i; ++j) {
            idx_t offset = i - pent_pos[j];
            seq_t prod = cache[offset] * (seq_t)pent_val[j];
            sum -= prod;
        }
        cache[i] = sum;
    }
    max_computed = (size_t)n;
}

/* ---------- public function ----------
 * Computes a(n) for any n≥0, caching results for efficiency. ----------
 */
seq_t a(idx_t n) {
    /* First invocation: allocate cache and initial pentagon data. */
    if (cache == NULL) {
        size_t init_cap = (size_t)n + 1 > 1 ? (size_t)n + 1 : 1;
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) abort();
        cache_cap = init_cap;
        cache[0] = 1;
        build_pentaginals(cache_cap);
        max_computed = 0;
    }

    if ((size_t)n > max_computed)
        compute_up_to(n);

    return cache[(size_t)n];
}

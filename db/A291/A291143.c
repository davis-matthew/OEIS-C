/*
 * A291143  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S^3)^3.
 * https://oeis.org/A291143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291143: p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S^3)^3.
   Recurrence for n ≥ 10:
     a(n) = 9*a(n-1) - 36*a(n-2) + 87*a(n-3) - 144*a(n-4) + 171*a(n-5)
          - 147*a(n-6) + 90*a(n-7) - 36*a(n-8) + 8*a(n-9)
   Initial values:
     a(0)=0, a(1)=0, a(2)=3, a(3)=9, a(4)=18, a(5)=36, a(6)=81,
     a(7)=189, a(8)=430, a(9)=954 */
#ifdef __cplusplus
#  include <vector>
#  include <cstddef>
#else
#  include <stdlib.h>
#  include <stddef.h>
#endif

static const int coeff[9] = {9, -36, 87, -144, 171, -147, 90, -36, 8};
static const seq_t init[10] = {
    0, 0, 3, 9, 18, 36, 81, 189, 430, 954
};

#ifdef __cplusplus
static std::vector<seq_t> cache;
static size_t cache_len = 0;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
#endif

static void ensure_cache(idx_t n)
{
    if (n < 10) return;                       /* values already known */

#ifdef __cplusplus
    /* C++ implementation: use std::vector */
    if (cache.empty()) {
        cache.resize(10);
        cache[0] = 0; cache[1] = 0; cache[2] = 3;
        cache[3] = 9; cache[4] = 18; cache[5] = 36;
        cache[6] = 81; cache[7] = 189; cache[8] = 430;
        cache[9] = 954;
        cache_len = 10;
    }
    if (cache.size() <= (size_t)n) {
        size_t new_cap = cache.size();
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache.resize(new_cap);
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 10) continue;   /* already initialized */
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[(size_t)(i - 1 - k)];
            int c = coeff[k];
            seq_t prod = term * (seq_t)(c > 0 ? c : -c);
            if (c > 0) pos += prod;
            else      neg += prod;
        }
        cache[(size_t)i] = pos - neg;   /* pos ≥ neg by construction */
    }
    if (cache_len <= (size_t)n) cache_len = (size_t)n + 1;
#else
    /* C implementation: use malloc/realloc */
    if (!cache) {
        cache_cap = 10;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 0; cache[1] = 0; cache[2] = 3;
        cache[3] = 9; cache[4] = 18; cache[5] = 36;
        cache[6] = 81; cache[7] = 189; cache[8] = 430;
        cache[9] = 954;
        cache_len = 10;
    }
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 10) continue;   /* already initialized */
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[(size_t)(i - 1 - k)];
            int c = coeff[k];
            seq_t prod = term * (seq_t)(c > 0 ? c : -c);
            if (c > 0) pos += prod;
            else      neg += prod;
        }
        cache[(size_t)i] = pos - neg;   /* pos ≥ neg by construction */
    }
    if (cache_len <= (size_t)n) cache_len = (size_t)n + 1;
#endif
}

seq_t a(idx_t n)
{
    if (n < 10) return init[(size_t)n];
    ensure_cache(n);
    return cache[(size_t)n];
}

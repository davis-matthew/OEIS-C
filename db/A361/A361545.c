/*
 * A361545  Expansion of e.g.f. exp(x^4/(24 * (1-x))).
 * https://oeis.org/A361545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A361545: Expansion of e.g.f. exp(x^4/(24*(1-x))) .
 * Recurrence (n > 4):
 *   a(n) = 2*(n-1)*a(n-1) - (n-1)*(n-2)*a(n-2)
 *          + C(n-1,3)*a(n-4) - 3*C(n-1,4)*a(n-5)
 * Base values: a(0)=1, a(1)=a(2)=a(3)=0, a(4)=1
 * The recurrence is computed with order
 *   (2*(n-1)*a(n-1) + C(n-1,3)*a(n-4))
 *   - ( (n-1)*(n-2)*a(n-2) + 3*C(n-1,4)*a(n-5) )
 * so that the intermediate sum is never negative.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static bool cache_inited = false;

static void init_cache(void) {
    cache_inited = true;
    cache_vec.resize(5);
    cache_vec[0] = 1;
    cache_vec[1] = 0;
    cache_vec[2] = 0;
    cache_vec[3] = 0;
    cache_vec[4] = 1;
}

/* Compute a single index (i>4) given that all previous terms exist */
static inline void compute_cxx(idx_t i) {
    seq_t nm1 = (seq_t)(i - 1);
    seq_t nm2 = (seq_t)(i - 2);
    seq_t nm3 = (seq_t)(i - 3);
    seq_t nm4 = (seq_t)(i - 4);

    seq_t binom3 = nm1 * nm2 * nm3 / 6;          /* C(n-1,3) */
    seq_t binom4 = nm1 * nm2 * nm3 * nm4 / 24;   /* C(n-1,4) */

    seq_t a1 = cache_vec[i-1];
    seq_t a2 = cache_vec[i-2];
    seq_t a4 = cache_vec[i-4];
    seq_t a5 = cache_vec[i-5];

    seq_t pos = 2 * nm1 * a1 + binom3 * a4;
    seq_t sub = nm1 * nm2 * a2 + 3 * binom4 * a5;

    cache_vec[i] = pos - sub;
}
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow cache to hold indices up to n (inclusive) */
static inline void grow_cache_to(idx_t n) {
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? need : cache_cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) abort();          /* out‑of‑memory */
        cache = newptr;
        cache_cap = new_cap;
    }
}

static void init_cache(void) {
    grow_cache_to(4);
    cache[0] = 1;
    cache[1] = 0;
    cache[2] = 0;
    cache[3] = 0;
    cache[4] = 1;
    cache_len = 5;
}

/* Compute a single index (i>4) given that all previous terms exist */
static inline void compute_c(idx_t i) {
    seq_t nm1 = (seq_t)(i - 1);
    seq_t nm2 = (seq_t)(i - 2);
    seq_t nm3 = (seq_t)(i - 3);
    seq_t nm4 = (seq_t)(i - 4);

    seq_t binom3 = nm1 * nm2 * nm3 / 6;          /* C(n-1,3) */
    seq_t binom4 = nm1 * nm2 * nm3 * nm4 / 24;   /* C(n-1,4) */

    seq_t a1 = cache[i-1];
    seq_t a2 = cache[i-2];
    seq_t a4 = cache[i-4];
    seq_t a5 = cache[i-5];

    seq_t pos = 2 * nm1 * a1 + binom3 * a4;
    seq_t sub = nm1 * nm2 * a2 + 3 * binom4 * a5;

    cache[i] = pos - sub;
}
#endif

seq_t a(idx_t n) {
#if defined(__cplusplus)
    if (!cache_inited) init_cache();
    if (n >= cache_vec.size()) {
        cache_vec.resize(n + 1);
        for (idx_t i = 5; i <= n; ++i)   /* indices <5 already initialized */
            compute_cxx(i);
    }
    return cache_vec[n];
#else
    if (cache_len == 0) init_cache();
    if (n >= cache_cap) grow_cache_to(n);
    if (n + 1 > cache_len) { /* compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 4) continue;   /* base values already set */
            compute_c(i);
        }
        cache_len = (size_t)n + 1;
    }
    return cache[n];
#endif
}

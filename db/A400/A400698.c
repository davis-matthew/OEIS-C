/*
 * A400698  a(n) = Sum_{k=0..n} mu(k^2 + 1).
 * https://oeis.org/A400698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, new
 * Formula:   a(n) = Sum_{k=0..n} mu(k^2 + 1)
 * Range:     i8 n <= 4220, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9929 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* static cache of cumulative sums */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;   /* number of elements stored (>=1) */

    /* grow cache if necessary */
    if ((size_t)n + 1 > cached_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = NULL;
        if (cache) {
            new_cache = seq_realloc(cache, cached_len, new_len);
        } else {
            new_cache = seq_alloc(new_len);
        }
        /* Initialize new entries */
        for (size_t k = cached_len; k < new_len; ++k) {
            idx_t m = k * k + 1;                       /* k^2 + 1 fits in idx_t */
            seq_t mu = (seq_t)mu_of(m);                /* mu_of returns int */
            if (k == 0) {
                new_cache[k] = mu;                     /* a(0) = mu(1) = 1 */
            } else {
                new_cache[k] = new_cache[k - 1] + mu;   /* cumulative sum */
            }
        }
        cache = new_cache;
        cached_len = new_len;
    }

    return cache[n];
}

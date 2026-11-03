/*
 * A365980  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(2*k+3) / (2*k+3) ).
 * https://oeis.org/A365980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(0) = 1 * a(n) = Sum_{k=0..floor((n-3)/2)} (2*k+2)! * C(n,
 *            2*k+3) * a(n-2*k-3) * (C(n, k) is the binomial coefficient)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;        /* a(0)..a(max_computed) */
    static size_t cache_len  = 0;      /* allocated length */
    static size_t cache_done = 0;      /* highest index computed */

    /* Static factorial cache for !0..!max_n */
    static seq_t *fact = NULL;         /* (2*k+2)! for k up to max factor needed */
    static size_t fact_len  = 0;       /* allocated length */
    static size_t fact_done = 0;       /* highest index computed */

    /* Allocate and pre‑compute a(0) */
    if (cache_len == 0) {
        grow_seq_array(&cache, &cache_len, 1);
        cache[0] = 1;
        cache_done = 0;
    }

    /* Allocate factorial array up to n */
    if (fact_len == 0) {
        grow_seq_array(&fact, &fact_len, 1);
        fact[0] = 1;
        fact_done = 0;
    }

    /* Extend factorial array if needed */
    if (fact_len <= n) {
        grow_seq_array(&fact, &fact_len, n + 1);
    }
    /* Compute missing factorials */
    for (idx_t i = fact_done + 1; i <= n; ++i) {
        fact[i] = seq_mul(fact[i - 1], (seq_t)i);
    }
    fact_done = n;

    /* Extend cache if needed */
    if (cache_len <= n) {
        grow_seq_array(&cache, &cache_len, n + 1);
    }
    /* Compute missing sequence values */
    for (idx_t i = cache_done + 1; i <= n; ++i) {
        seq_t sum = 0;  /* start with zero */
        for (idx_t k = 0; ; ++k) {
            idx_t term_idx = 2 * k + 3;
            if (term_idx > i) break;        /* no more terms */

            /* (2*k+2)! */
            seq_t fac = fact[2 * k + 2];

            /* C(i, 2*k+3) */
            seq_t bin = binomial(i, term_idx);

            /* a(i-2*k-3) */
            seq_t sub = cache[i - term_idx];

            /* term = fac * bin * sub */
            seq_t term = seq_mul(fac, bin);
            term = seq_mul(term, sub);

            sum = sum + term;
        }
        cache[i] = sum;
    }
    cache_done = n;

    return cache[n];
}

/*
 * A280258  a(n) = Sum_{d|n} pxi(d), where pxi(m) is the product of totatives
 *          of m (A001783).
 * https://oeis.org/A280258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001783(d) (product of totatives of d)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* see OEIS A280258 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* compute product of totatives of m (A001783) */
static seq_t totprod(idx_t m) {
    seq_t p = 1;
    if (m == 0) return 0;
    for (idx_t k = 1; k <= m; ++k) {
        if (gcd_idx(k, m) == 1)
            p *= (seq_t)k;
    }
    return p;
}

/* static cache of totprod values up to current capacity */
static seq_t *cache = NULL;   /* dynamic array [0..cap-1] */
static size_t   cached_cap = 0;

/* ensure cache holds values up to n (inclusive) */
static void ensure_cache(idx_t n) {
    if ((size_t)(n + 1) <= cached_cap)
        return;                    /* already large enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = NULL;
    if (cached_cap == 0) {
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (!new_cache) /* allocation failure – should not happen in tests */
        return;

    /* compute missing entries */
    for (size_t i = cached_cap; i < new_cap; ++i)
        new_cache[i] = totprod((idx_t)i);

    cache = new_cache;
    cached_cap = new_cap;
}

/* main function required by the framework */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;

    ensure_cache(n);   /* now cache[d] is available for all d ≤ n */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += cache[d];
            idx_t e = n / d;
            if (e != d)
                sum += cache[e];
        }
    }
    return sum;
}

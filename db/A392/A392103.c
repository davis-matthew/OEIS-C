/*
 * A392103  E.g.f. A(x) satisfies A(x) = (1 + 5*x*A(x)^2)^(1/5).
 * https://oeis.org/A392103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A392103 : OEIS sequence  A392103
    Recurrence:  a(n+5) = -6*(3*n-1)*(2*n+1)*(3*n+4)*(n+3)^2 * a(n)
    Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-6, a(4)=24
    a(5*n+2)=0 follows automatically from the recurrence.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

/* Cache holding previously computed terms.  Memory is grown lazily. */
static seq_t *cache = NULL;      /* array of seq_t */
static idx_t  cache_cap = 0;     /* current capacity (size) */
static bool   init_done = false;

/* Make sure the cache can address indices up to n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n + 1 <= cache_cap) return;
    size_t new_cap = n + 1;
    if (!cache) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    /* New entries are zero‑initialised by seq_alloc/seq_realloc. */
    cache_cap = new_cap;
}

/* Initialise the first five terms once. */
static void initialise_terms(void) {
    if (init_done) return;
    ensure_cache(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)-6;
    cache[4] = (seq_t)24;
    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    initialise_terms();
    ensure_cache(n);

    /* Return cached value if already computed. */
    if (n < cache_cap && (n == 0 || n == 1 || n == 2 || n == 3 || n == 4))
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        if (i < 5) continue;                 /* safety for small n */

        /* i corresponds to n+5 in the recurrence → n = i-5 */
        idx_t m = i - 5;                      /* current "n" in recurrence */

        /* Compute the multiplicative factor:
           factor = 6 * |3*m-1| * (2*m+1) * (3*m+4) * (m+3)^2
           The overall sign is -sgn(3*m-1); all other terms are positive.   */
        int64_t three_m_minus_1 = (int64_t)3 * (int64_t)m - 1;
        uint64_t abs_three_m_minus_1 = three_m_minus_1 >= 0
                                       ? (uint64_t)three_m_minus_1
                                       : (uint64_t)(-three_m_minus_1);

        int sign = three_m_minus_1 >= 0 ? -1 : 1;   /* sign of - (3*m-1) */

        seq_t prod = (seq_t)1;
        prod = mul_small(prod, 6);
        prod = mul_small(prod, (unsigned int)abs_three_m_minus_1);
        prod = mul_small(prod, (unsigned int)(2 * m + 1));
        prod = mul_small(prod, (unsigned int)(3 * m + 4));

        /* (m+3)^2 */
        uint64_t tmp = m + 3;
        prod = mul_small(prod, (unsigned int)tmp);
        prod = mul_small(prod, (unsigned int)tmp);

        /* Multiply by the previous term a(m). */
        prod = seq_mul(prod, cache[m]);

        /* Apply the final sign. */
        if (sign < 0)
            cache[i] = -prod;
        else
            cache[i] = prod;
    }

    return cache[n];
}

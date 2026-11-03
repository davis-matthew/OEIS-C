/*
 * A372161  E.g.f. A(x) satisfies A(x) = exp( 3 * x / (1 - x * A(x)^(1/3)) ).
 * https://oeis.org/A372161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * n! * Sum_{k=1..n} (n-k+3)^(k-1) * binom(n-1, k-1) /
 *            k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372161: a(0)=1, a(1)=3, a(2)=15, a(3)=117, ... */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* cache for computed a(n) values */
static seq_t *a_cache  = NULL;   /* memoised a(n) values */
static idx_t  a_cap    = 0;

/* factorial computed on the fly */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        ensure_cache(1, &a_cache, &a_cap);
        if (a_cache[0] == 0)
            a_cache[0] = 1;
        return a_cache[0];
    }

    /* return cached value if available */
    ensure_cache(n + 1, &a_cache, &a_cap);
    if (a_cache[n] != 0)
        return a_cache[n];

    /* compute n! once */
    seq_t fac_n = factorial(n);

    /* initial values for the iteration */
    seq_t binom = 1;                /* C(n-1,0)      */
    seq_t F     = fac_n;            /* n! / 1!      */
    seq_t sum   = 0;
    const idx_t N = n;

    for (idx_t k = 1; k <= N; ++k) {
        /* base = n - k + 3, exponent = k-1 */
        seq_t base = (seq_t)(N - k + 3);
        idx_t  exp = k - 1;
        seq_t pow_val = ipow(base, (seq_t)exp);

        /* term = 3 * F * binom * base^(k-1) */
        seq_t term = 3 * F * binom;
        term = term * pow_val;

        sum += term;

        if (k == N)
            break;

        /* update F: divide by k+1  (F = n! / (k+1)!) */
        F = F / ((seq_t)(k + 1));

        /* update binom: binom = C(n-1,k) = binom * (n - k) / k */
        binom = binom * (seq_t)(N - k);
        binom = binom / (seq_t)k;
    }

    a_cache[n] = sum;
    return sum;
}

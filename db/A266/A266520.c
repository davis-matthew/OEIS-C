/*
 * A266520  E.g.f.: log( Sum_{n>=0} (n+1)^(2*n) * x^n/n! ).
 * https://oeis.org/A266520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n/n! in log( Sum_{k>=0} (k+1)^(2k) *
 *            x^k/k! ) Recurrence: a(1) = f(1), a(n) = f(n) - Σ_{k=1}^{n-1}
 *            binom(n-1,k-1) * a(k) * f(n-k) (n≥2) where f(k) = (k+1)^(2k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

static seq_t *f_cache = NULL;   /* f(k) = (k+1)^(2k) */
static seq_t *a_cache = NULL;   /* a(k) values */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure that f_cache and a_cache can store indices 0..n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    f_cache = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));

    /* Base value: f(0) = 1. a(0) is unused. */
    f_cache[0] = (seq_t)1;

    cache_cap = new_cap;
}

/* Compute a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    ensure_cache(n);

    /* Compute f(k) and a(k) for k = 1..n. */
    for (idx_t cur = 1; cur <= n; ++cur)
    {
        /* f(cur) = (cur+1)^(2*cur) */
        f_cache[cur] = pow_seq((seq_t)(cur + 1), (int)(2 * cur));

        if (cur == 1)
        {
            a_cache[1] = f_cache[1];
            continue;
        }

        /* a(cur) = f(cur) - Σ_{k=1}^{cur-1} binom(cur-1,k-1) * a(k) * f(cur-k) */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;   /* binom(cur-1,0) */

        for (idx_t k = 1; k < cur; ++k)
        {
            seq_t term = a_cache[k];
            term *= f_cache[cur - k];
            term *= bin;
            sum += term;

            /* Update bin to binom(cur-1,k) for next iteration. */
            if (k < cur - 1)
            {
                bin *= (seq_t)(cur - k);
                bin /= (seq_t)k;   /* exact division */
            }
        }
        a_cache[cur] = f_cache[cur] - sum;
    }

    return a_cache[n];
}

/*
 * A365155  G.f. satisfies A(x) = ( 1 + x*A(x)^2*(1 + x*A(x))^2 )^2.
 * https://oeis.org/A365155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(2*(n+k+1), k) * binomial(2*k,
 *            n-k)/(n+k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via the recurrence derived from the functional equation:  */
/*   a(0) = 1                                                    */
/*   h(i)   = Σ_{k=0..i-1} c(k) * d(i-1-k)   (i ≥ 1)             */
/*   a(i)   = Σ_{k=0..i}   h(k) * h(i-k)                           */
/*   c(i)   = Σ_{p+q=i}    a(p) * a(q)                            */
/*   d(0)   = 1                                                   */
/*   d(1)   = 2*a(0)                                               */
/*   d(i)   = 2*a(i-1) + c(i-2)   (i ≥ 2)                          */
/* All arrays are lazily computed and cached.                       */

#include <stddef.h>
#include "../utils/seq_alloc.h"   /* seq_alloc, seq_realloc */
#include "../utils/seq_realloc.h"

static seq_t *a_cache = NULL;   /* a[n]   */
static seq_t *h_cache = NULL;   /* h[n]   */
static seq_t *c_cache = NULL;   /* c[n]   */
static seq_t *d_cache = NULL;   /* d[n]   */

static size_t cache_cap = 0;     /* size of each allocated array      */
static idx_t  last_computed = (idx_t)-1;

/* Allocate or enlarge the cached arrays to hold indices up to n */
static void ensure_cap(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

        if (a_cache == NULL)        a_cache = seq_alloc(new_cap);
        else                         a_cache = seq_realloc(a_cache, cache_cap, new_cap);

        if (h_cache == NULL)        h_cache = seq_alloc(new_cap);
        else                         h_cache = seq_realloc(h_cache, cache_cap, new_cap);

        if (c_cache == NULL)        c_cache = seq_alloc(new_cap);
        else                         c_cache = seq_realloc(c_cache, cache_cap, new_cap);

        if (d_cache == NULL)        d_cache = seq_alloc(new_cap);
        else                         d_cache = seq_realloc(d_cache, cache_cap, new_cap);

        cache_cap = new_cap;
    }
}

/* Return a(n) for any n ≥ 0, with memoisation and O(n²) total work.   */
seq_t a(idx_t n)
{
    /* initialise base case if not yet done */
    if (last_computed == (idx_t)-1) {
        ensure_cap(0);
        a_cache[0] = (seq_t)1;
        h_cache[0] = (seq_t)1;
        c_cache[0] = (seq_t)1;          /* a0 * a0 */
        d_cache[0] = (seq_t)1;          /* 1                     */
        last_computed = 0;
    }

    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            ensure_cap(i);

            /* compute h[i] = Σ_{k=0..i-1} c[k] * d[i-1-k] */
            seq_t sum_h = (seq_t)0;
            for (idx_t k = 0; k < i; ++k)
                sum_h += c_cache[k] * d_cache[i - 1 - k];
            h_cache[i] = sum_h;

            /* compute a[i] = Σ_{k=0..i} h[k] * h[i-k] */
            seq_t sum_a = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k)
                sum_a += h_cache[k] * h_cache[i - k];
            a_cache[i] = sum_a;

            /* compute c[i] = Σ_{p+q=i} a[p] * a[q] */
            seq_t sum_c = (seq_t)0;
            for (idx_t p = 0; p <= i; ++p)
                sum_c += a_cache[p] * a_cache[i - p];
            c_cache[i] = sum_c;

            /* compute d[i] */
            if (i == 0) {
                d_cache[0] = (seq_t)1;
            } else if (i == 1) {
                d_cache[1] = (seq_t)2 * a_cache[0];
            } else {
                d_cache[i] = (seq_t)2 * a_cache[i - 1] + c_cache[i - 2];
            }
        }
        last_computed = n;
    }
    return a_cache[n];
}

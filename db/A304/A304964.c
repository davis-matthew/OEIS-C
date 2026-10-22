/*
 * A304964  Expansion of 1/(1 - Sum_{i>=1, j>=1, k>=1, l>=1} x^(i*j*k*l)).
 * https://oeis.org/A304964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = Σ_{k=1..n} d4(k) * a(n-k) (n ≥ 1) where d4(k) =
 *            ∏_{p^e‖k} C(e+3,3) (number of ordered 4‑tuples of positive
 *            integers whose product is k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine below memoises the sequence values and the helper
   values d4(k).  It only uses C standard library and the
   helper functions supplied by the build.  It works for any
   seq_t/idx_t types defined by the test harness. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/binom.h"

static seq_t *a_cache = NULL;    /* a values */
static seq_t *d4_cache = NULL;   /* d4 values */
static idx_t *spf         = NULL;
static idx_t   spf_cap    = 0;
static idx_t   cap        = 0;   /* current allocated size (>=1) */

/* compute d4(k) = ∑_{p^e‖k} C(e+3,3) using SPF table */
static seq_t d4_value(idx_t k)
{
    seq_t prod = (seq_t)1;
    idx_t m = k;
    while (m > 1) {
        idx_t p = spf[m];
        idx_t e = 0;
        while (m % p == 0) {
            ++e;
            m /= p;
        }
        prod = prod * binom(e + 3, 3);
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cap == 0) {
        cap = 1;
        a_cache  = (seq_t *)malloc(sizeof(seq_t) * cap);
        d4_cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        a_cache[0] = (seq_t)1;
        d4_cache[0] = (seq_t)0;   /* unused */
    }

    /* Expand cache if needed */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        a_cache  = (seq_t *)realloc(a_cache,  new_cap * sizeof(seq_t));
        d4_cache = (seq_t *)realloc(d4_cache, new_cap * sizeof(seq_t));
        cap = (idx_t)new_cap;
    }

    /* Extend SPF table if necessary */
    if (spf_cap <= n) {
        idx_t *new_spf;
        build_spf(n, &new_spf);   /* build_spf allocates array of size n+1 */
        free(spf);                /* free previous table if any */
        spf = new_spf;
        spf_cap = n + 1;
    }

    /* Compute d4 for new indices */
    for (idx_t k = (idx_t)(cap - 1); k <= n; ++k)
        d4_cache[k] = d4_value(k);

    /* Compute a for new indices */
    for (idx_t i = (idx_t)(cap - 1); i <= n; ++i) {
        if (i == 0) { /* already initialized */
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += d4_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum;
    }

    return a_cache[n];
}

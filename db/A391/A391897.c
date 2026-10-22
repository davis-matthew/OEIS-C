/*
 * A391897  Partial products of A063659.
 * https://oeis.org/A391897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=1..n} a(k), where a(k) = Σ_{d|k} μ(k/d) ψ(d) ψ is
 *            Dedekind ψ(n) = n Π_{p|n} (1+1/p). Equivalently, a(k) is
 *            multiplicative: If k = ∏ p^e, then a(k) = ∏ a(p^e), where a(p^e)
 *            = p if e==1, p^(e-2) * (p^2-1) if e>=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a smallest‑prime‑factor (spf) sieve for fast factorisation.
   We compute the partial products a(1) … a(n) on demand and cache them.
   All operations are done on type seq_t; integer constants are cast to seq_t
   where appropriate. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"          /* build_spf() */

static seq_t *prod_cache = NULL;   /* partial products, 1‑based */
static size_t prod_cap   = 0;        /* allocated size */
static idx_t  prod_max   = 0;        /* largest n computed */

static idx_t *spf        = NULL;    /* smallest prime factor array */
static idx_t  spf_limit  = 0;        /* current limit of spf array */

/* Ensure the spf array includes the number n.  Rebuild if necessary. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    free(spf);
    build_spf(n, &spf);
    spf_limit = n;
}

/* Compute a(k) using its prime‑power factorisation. */
static seq_t a_of(idx_t k)
{
    if (k <= 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp   = k;

    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        seq_t part;
        if (e == 1) {
            part = (seq_t)p;
        } else {
            /* part = p^(e-2) * (p^2 - 1) */
            seq_t p2   = (seq_t)p * (seq_t)p;   /* p^2   */
            seq_t diff = p2 - (seq_t)1;          /* p^2-1 */
            part = diff;                         /* start with p^2-1 */

            /* multiply by p^(e-2) */
            for (idx_t i = 0; i < e - 2; ++i) {
                part *= (seq_t)p;
            }
        }
        result *= part;
    }
    return result;
}

/* Public sequence value. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for n<1 */

    /* Extend cache if needed. */
    if (n >= prod_cap) {
        size_t new_cap = (size_t)n + 1;         /* indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(prod_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        prod_cache = tmp;
        prod_cap   = new_cap;
    }

    /* Return cached value if already computed. */
    if (prod_max >= n) return prod_cache[n];

    ensure_spf(n);

    /* Compute missing partial products. */
    for (idx_t i = prod_max + 1; i <= n; ++i) {
        seq_t ak = a_of(i);
        if (i == 1) {
            prod_cache[i] = ak;                 /* a(1)=1 */
        } else {
            prod_cache[i] = prod_cache[i - 1] * ak;
        }
    }

    prod_max = n;
    return prod_cache[n];
}

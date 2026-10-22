/*
 * A399685  a(n) = Product_{k=1..n} sigma(k,k).
 * https://oeis.org/A399685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1…n} σ(k,k) where σ(k,k)=∑_{d|k} d^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   build_spf, seq_alloc, seq_mul, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_spf.h"

/* Global static state – visible to all functions in this file */
static seq_t *prod_cache   = NULL;  /* cache of a(0)…a(max_computed)        */
static size_t cache_cap    = 0;     /* current capacity of prod_cache        */
static idx_t  max_computed = 0;     /* highest n that has been computed      */

static idx_t *spf  = NULL;         /* smallest‑prime‑factor array (≤ n) */
static size_t spf_cap = 0;           /* capacity of spf array                */

/* ------------------------------------------------------------------ */
/* σ(k,k) – sum of d^k over all divisors d of k, using the SPF array   */
/* ------------------------------------------------------------------ */
static seq_t sigma_k(idx_t k)
{
    if (k == 1) return (seq_t)1;          /* σ(1,1) = 1 */

    uint64_t x   = (uint64_t)k;
    seq_t    prod = (seq_t)1;

    while (x > 1) {
        uint64_t p = (uint64_t)spf[x];
        uint64_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }

        /* pk = p^k */
        seq_t pk = seq_pow((seq_t)p, k);

        /* sum = 1 + pk + pk^2 + … + pk^exp */
        seq_t term = (seq_t)1;
        seq_t sum  = (seq_t)1;
        for (uint64_t e = 1; e <= exp; ++e) {
            term = seq_mul(term, pk);
            sum  += term;
        }

        prod = seq_mul(prod, sum);
    }

    return prod;
}

/* ------------------------------------------------------------------ */
/* a(n) – product of σ(k,k) for k=1…n, with offset a(0)=1             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise cache First‑time call */
    if (!prod_cache) {
        prod_cache = seq_alloc(1);            /* a(0) only */
        if (!prod_cache) return (seq_t)0;     /* allocation failure */
        prod_cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_cap    = 1;
        max_computed = 0;
    }

    /* Make sure SPF table covers [1..n] */
    if ((size_t)n > spf_cap) {
        build_spf((idx_t)n, &spf);
        spf_cap = (size_t)n;
    }

    /* Grow product cache and compute missing terms if necessary */
    if (n > max_computed) {
        prod_cache = seq_realloc(prod_cache, cache_cap, (size_t)n + 1);
        if (!prod_cache) return (seq_t)0;         /* allocation failure */
        cache_cap = (size_t)n + 1;

        for (idx_t k = max_computed + 1; k <= n; ++k) {
            prod_cache[k] = seq_mul(prod_cache[k - 1], sigma_k(k));
        }
        max_computed = n;
    }

    return prod_cache[n];
}

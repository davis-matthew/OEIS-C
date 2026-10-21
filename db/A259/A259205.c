/*
 * A259205  E.g.f.: Sum_{n>=0} x^n * (2 + x^n)^n * exp(3*x^(n+1)) / n!.
 * https://oeis.org/A259205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A259205
 *  a(n) = n! * Σ_{k,i,j≥0} binom(k,i) · 2^{k-i} · 3^j · 1/(k! j!)
 *            where n = k·(1+i) + j·(k+1)
 *  The code maintains factorials, powers of 2 and 3 in static arrays,
 *  grows them on demand, and memoises computed a(n) values.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *fac = NULL;     /* factorials */
static seq_t *pow2 = NULL;    /* powers of 2 */
static seq_t *pow3 = NULL;    /* powers of 3 */
static size_t fac_cap = 0;    /* current capacity of the arrays */

/* grow the factorial and power arrays so that indices up to n are valid */
static void ensure_arrays(idx_t n)
{
    if (fac_cap > (size_t)n) return;

    size_t newcap = fac_cap ? fac_cap : 1;
    while (newcap <= (size_t)n) newcap <<= 1;

    seq_t *newfac  = (seq_t *)realloc(fac, newcap * sizeof(seq_t));
    seq_t *newpow2 = (seq_t *)realloc(pow2, newcap * sizeof(seq_t));
    seq_t *newpow3 = (seq_t *)realloc(pow3, newcap * sizeof(seq_t));
    if (!newfac || !newpow2 || !newpow3) { exit(1); }

    if (!fac_cap) {                    /* first allocation */
        newfac[0]  = (seq_t)1;
        newpow2[0] = (seq_t)1;
        newpow3[0] = (seq_t)1;
        size_t i;
        for (i = 1; i < newcap; ++i) {
            newfac[i]  = newfac[i-1] * (seq_t)i;
            newpow2[i] = newpow2[i-1] * (seq_t)2;
            newpow3[i] = newpow3[i-1] * (seq_t)3;
        }
    } else {                            /* extend existing arrays */
        size_t i;
        for (i = fac_cap; i < newcap; ++i) {
            newfac[i]  = newfac[i-1] * (seq_t)i;
            newpow2[i] = newpow2[i-1] * (seq_t)2;
            newpow3[i] = newpow3[i-1] * (seq_t)3;
        }
    }

    fac  = newfac;
    pow2 = newpow2;
    pow3 = newpow3;
    fac_cap = newcap;
}

/* static cache of computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* capacity of cache array */

/* grow cache up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;
    size_t newcap = cache_cap ? cache_cap : 1;
    while (newcap <= (size_t)n) newcap <<= 1;
    seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newc) exit(1);
    cache = newc;
    cache_cap = newcap;
}

/* compute the nth term of A259205 */
seq_t a(idx_t n)
{
    /* ensure necessary factorials and powers are available */
    ensure_arrays(n);
    /* ensure cache can store up to n */
    ensure_cache(n);
    /* if already computed, return cached value */
    if (n < cache_cap && cache[n] != (seq_t)0) return cache[n];

    seq_t result = 0;
    seq_t fact_n = fac[(size_t)n];

    /* outer sum over k */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t temp = fact_n / fac[(size_t)k];   /* n! / k!  */
        /* binomial coefficient for i=0 .. k */
        seq_t binom = 1;                        /* binom(k,0) */
        for (idx_t i = 0; i <= k; ++i) {
            idx_t prod_k = k * (i + 1);
            if (prod_k > n) break;             /* further i only increase prod_k */
            idx_t rem = n - prod_k;
            idx_t div = k + 1;
            if (rem % div == 0) {
                idx_t j = rem / div;
                /* term factor: n! / (k! * j!)  */
                seq_t term_factor = temp / fac[(size_t)j];
                seq_t term = binom;
                term *= pow2[(size_t)(k - i)];
                term *= pow3[(size_t)j];
                term *= term_factor;
                result += term;
            }
            /* update binomial for next i */
            if (i < k) {
                binom = binom * (seq_t)(k - i);
                binom = binom / (seq_t)(i + 1);
            }
        }
    }

    /* store in cache (cache[0] is 0 by convention; we store after computation) */
    cache[n] = result;
    return result;
}

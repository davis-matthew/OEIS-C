/*
 * A353191  Expansion of e.g.f. 1/(1 - Sum_{k>=1} mu(k) * x^k / k), where mu()
 *          is the Moebius function (A008683).
 * https://oeis.org/A353191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   binomial, mem, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS formula: a(0) = 1; a(n) = Sum_{k=1..n} (k-1)! * mu(k) *
 *    binomial(n,k) * a(n-k)
 *    (mu = Möbius function, binomial = n choose k)
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mobius.h"
#include "../utils/mem.h"

/* Cached data structures */
static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) */
static size_t cache_cap = 0;
static seq_t *fact = NULL;      /* fact[i] = i! */
static size_t fact_cap = 0;
static idx_t computed_max = 0;

/* Ensure that a_cache can hold indices up to 'need' */
static void ensure_cache(size_t need)
{
    if (need >= cache_cap) {
        size_t new_cap = need + 1;
        if (new_cap < 2 * cache_cap)
            new_cap = 2 * cache_cap;
        if (new_cap < 1) new_cap = 1;
        a_cache = mem_resize(seq_t, a_cache, new_cap);
        cache_cap = new_cap;
    }
}

/* Ensure that fact can hold factorials up to 'need' */
static void ensure_fact(size_t need)
{
    if (need >= fact_cap) {
        size_t new_cap = need + 1;
        if (new_cap < 2 * fact_cap)
            new_cap = 2 * fact_cap;
        if (new_cap < 1) new_cap = 1;
        fact = mem_resize(seq_t, fact, new_cap);
        fact_cap = new_cap;
    }
}

/* Pre‑compute factorials up to n and initialise a_cache[0] */
static void init_precomputation(idx_t n)
{
    ensure_cache((size_t)n);
    ensure_fact((size_t)n);

    /* factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    a_cache[0] = (seq_t)1;
}

/* Recursive computation with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n > computed_max) {
        init_precomputation(n);
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = fact[k - 1];          /* (k-1)!          */
                term *= mobius(k);                /* * mu(k)         */
                term *= binomial(i, k);           /* * choose(k,n)   */
                term *= a_cache[i - k];           /* * a(n-k)        */
                sum += term;
            }
            a_cache[i] = sum;
        }
        computed_max = n;
    }
    return a_cache[n];
}

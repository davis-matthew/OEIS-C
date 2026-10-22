/*
 * A350719  a(n) = Sum_{k=0..n} k! * 2^k * k^n * Stirling1(n,k).
 * https://oeis.org/A350719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k! * 2^k * k^n * Stirling1(n,k) (Stirling1
 *            refers to the signed Stirling numbers of the first kind)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility headers supplied by the build environment */
#include "../utils/seq_pow.h"
#include "../utils/seq_realloc.h"

/* Ensure a dynamic array has at least 'need' capacity. */
static void ensure_cap(size_t need, size_t *cap, seq_t **ptr)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newp = seq_realloc(*ptr, *cap, new_cap);
    *ptr = newp;
    *cap = new_cap;
}

/* Global static state for memoisation */
static idx_t    max_cached = (idx_t)-1;   /* largest n computed so far */
static seq_t  *s_cache    = NULL;        /* s(n,k) for current n (signed) */
static seq_t  *fact_cache = NULL;        /* k! for k <= n */
static seq_t  *pow2_cache = NULL;        /* 2^k for k <= n */
static seq_t  *powk_cache = NULL;        /* k^n for k <= n (current n) */
static seq_t  *a_cache    = NULL;        /* a(n) values */
static size_t  cap_s      = 0;
static size_t  cap_fact   = 0;
static size_t  cap_pow2   = 0;
static size_t  cap_powk   = 0;
static size_t  cap_a      = 0;

/* Helper: compute base^exp with the provided seq_pow utility. */
static seq_t seq_pow_k_idx(idx_t base, idx_t exp)
{
    return seq_pow((seq_t)base, exp);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Base case a(0) = 1, initialise all caches if necessary */
    if (n == 0) {
        if (max_cached == (idx_t)-1) {
            ensure_cap(1, &cap_s,     &s_cache);
            ensure_cap(1, &cap_fact,  &fact_cache);
            ensure_cap(1, &cap_pow2,  &pow2_cache);
            ensure_cap(1, &cap_powk,  &powk_cache);
            ensure_cap(1, &cap_a,     &a_cache);

            s_cache[0]     = (seq_t)1;   /* s(0,0)=1 */
            fact_cache[0]  = (seq_t)1;   /* 0! = 1   */
            pow2_cache[0]  = (seq_t)1;   /* 2^0 = 1  */
            powk_cache[0]  = (seq_t)1;   /* 0^0 = 1  */
            a_cache[0]     = (seq_t)1;   /* a(0) = 1 */
            max_cached     = 0;
        }
        return a_cache[0];
    }

    /* Return cached value if already computed */
    if (n <= max_cached)
        return a_cache[n];

    /* Grow all caches to accommodate up to n */
    ensure_cap((size_t)(n + 1), &cap_s,     &s_cache);
    ensure_cap((size_t)(n + 1), &cap_fact,  &fact_cache);
    ensure_cap((size_t)(n + 1), &cap_pow2,  &pow2_cache);
    ensure_cap((size_t)(n + 1), &cap_powk,  &powk_cache);
    ensure_cap((size_t)(n + 1), &cap_a,     &a_cache);

    /* Compute successive n from max_cached+1 up to the requested n */
    for (idx_t cur = max_cached + 1; cur <= n; ++cur) {
        /* Update Stirling numbers s(cur,k) in place (signed recurrence) */
        s_cache[cur] = (seq_t)0;            /* prepare s(cur,cur) placeholder */
        for (idx_t k = cur; k >= 1; --k) {
            seq_t old_sk   = s_cache[k];      /* s(cur-1,k)     */
            seq_t old_skm1 = s_cache[k-1];    /* s(cur-1,k-1)   */
            s_cache[k] = old_skm1 - ((seq_t)(cur - 1) * old_sk); /* signed */
        }
        s_cache[0] = (seq_t)0;                /* s(cur,0)=0 for cur>0 */

        /* Update factorial and power‑of‑two tables */
        fact_cache[cur] = fact_cache[cur-1] * (seq_t)cur;
        pow2_cache[cur] = pow2_cache[cur-1] * (seq_t)2;

        /* Update k^cur for existing k < cur */
        /* first set k=0 -> 0^cur = 0 for cur>0 */
        powk_cache[0] = (seq_t)0;
        for (idx_t k = 1; k < cur; ++k) {
            powk_cache[k] = powk_cache[k] * (seq_t)k; /* k^cur = k^(cur-1)*k */
        }
        /* Compute cur^cur for the new k = cur */
        powk_cache[cur] = seq_pow_k_idx(cur, cur);

        /* Compute a(cur) using the defining sum */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= cur; ++k) {
            seq_t term = s_cache[k];
            term = term * fact_cache[k];
            term = term * pow2_cache[k];
            term = term * powk_cache[k];
            sum += term;
        }
        a_cache[cur] = sum;
    }

    max_cached = n;
    return a_cache[n];
}

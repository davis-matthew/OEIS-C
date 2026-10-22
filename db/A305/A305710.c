/*
 * A305710  Expansion of e.g.f. exp(sec(x)*exp(x) - 1).
 * https://oeis.org/A305710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   exp(sec(x)*exp(x) - 1) = Σ a(n)*x^n/n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

static seq_t *euler_cache = NULL;
static size_t euler_cap = 0;
static idx_t euler_computed = 0;

static seq_t *c_cache = NULL;
static size_t c_cap = 0;
static idx_t c_computed = 0;

static seq_t *a_cache = NULL;
static size_t a_cap = 0;
static idx_t a_computed = 0;

/* Compute Euler (secant) numbers euler[n] for n ≥ 0.
   euler[0] = 1, euler[odd] = 0.
   Recurrence derived from sec(x)·cos(x)=1. */
static void compute_euler_upto(idx_t n)
{
    if (euler_computed > n) return;

    ensure_cache(n, &euler_cache, &euler_cap);
    for (idx_t i = euler_computed; i <= n; ++i)
    {
        if (i == 0) {
            euler_cache[0] = (seq_t)1;
            continue;
        }
        seq_t positive = (seq_t)0;
        seq_t negative = (seq_t)0;
        seq_t binom   = (seq_t)1;                /* C(i,0) */
        for (idx_t k = 1; k <= i; ++k)
        {
            /* update binom to C(i,k) */
            binom = binom * (i - k + 1) / k;
            if ((k & 1ULL) == 0)                /* k even */
            {
                seq_t term = binom * euler_cache[i - k];
                if (((k >> 1) & 1ULL) == 0)     /* (k/2) even → +1 */
                    positive += term;
                else                            /* (k/2) odd  → -1 */
                    negative += term;
            }
        }
        /* euler[i] = negative - positive   (always non‑negative) */
        euler_cache[i] = negative - positive;
    }
    euler_computed = n + 1;
}

/* Compute c[n] = coefficient of f(x)=sec(x)*exp(x)-1 (n≥1). */
static void compute_c_upto(idx_t n)
{
    if (c_computed > n) return;

    ensure_cache(n, &c_cache, &c_cap);
    for (idx_t i = c_computed; i <= n; ++i)
    {
        if (i == 0) {                                     /* f(0)=0 */
            c_cache[0] = (seq_t)0;
            continue;
        }
        seq_t sumc  = (seq_t)0;
        seq_t binom = (seq_t)1;                /* C(i,0) */
        for (idx_t k = 0; k <= i; ++k)
        {
            if ((k & 1ULL) == 0)               /* k even, euler[k] exists */
            {
                seq_t term = binom * euler_cache[k];
                sumc += term;
            }
            if (k < i) {
                binom = binom * (i - k) / (k + 1);
            }
        }
        c_cache[i] = sumc;
    }
    c_computed = n + 1;
}

/* Compute a[n] – the sequence term. */
static void compute_a_upto(idx_t n)
{
    if (a_computed > n) return;

    ensure_cache(n, &a_cache, &a_cap);
    for (idx_t i = a_computed; i <= n; ++i)
    {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;                /* C(i-1,0) */
        for (idx_t j = 0; j < i; ++j)
        {
            seq_t term = a_cache[j];
            term = term * c_cache[i - j];
            term = term * binom;
            sum += term;

            if (j < i - 1) {                   /* update C(i-1, j+1) */
                binom = binom * (i - 1 - j) / (j + 1);
            }
        }
        a_cache[i] = sum;
    }
    a_computed = n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure all auxiliary tables are large enough. */
    compute_euler_upto(n);
    compute_c_upto(n);
    compute_a_upto(n);
    return a_cache[n];
}

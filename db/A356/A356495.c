/*
 * A356495  Expansion of e.g.f. Product_{k>0} B((k * x)^k) where B(x) =
 *          exp(exp(x)-1) = e.g.f. of Bell numbers.
 * https://oeis.org/A356495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A354892(k) * binomial(n-1,k-1) * a(n-k)
 *            where A354892(k) = k! * Σ_{d|k} d^k / (k/d)!. Initial value:
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Ensure array large enough for 'needed' elements. */
static void grow_if_needed(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed > *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ----- static caches ----------------------------------------------- */
    static seq_t *fact      = NULL;  /* factorials 0..max   */
    static size_t fact_cap  = 0;
    static idx_t  fact_max  = 0;

    static seq_t *wcache    = NULL;  /* A354892 values 0..max (0 unused) */
    static size_t w_cap     = 0;
    static idx_t  w_max     = 0;

    static seq_t *acache    = NULL;  /* a[0..max]           */
    static size_t acap      = 0;
    static idx_t  amax      = 0;
    /* --------------------------------------------------------------------- */

    /* --- ensure factorial array up to n ----------------------------------- */
    if (fact == NULL) {
        fact      = (seq_t *)malloc(sizeof(seq_t));
        fact_cap  = 1;
        fact[0]   = 1;
        fact_max  = 0;
    }
    if (fact_max < n) {
        grow_if_needed((size_t)(n + 1), &fact_cap, &fact);
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    /* --- compute A354892(k) for k up to n ------------------------------- */
    if (wcache == NULL) {
        wcache    = (seq_t *)malloc(sizeof(seq_t));
        w_cap     = 1;
        wcache[0] = 0;
        w_max     = 0;
    }
    if (w_max < n) {
        grow_if_needed((size_t)(n + 1), &w_cap, &wcache);
        for (idx_t k = w_max + 1; k <= n; ++k) {
            seq_t wk = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    idx_t d2 = k / d;
                    /* divisor d */
                    seq_t mult1 = fact[k] / fact[d2];
                    seq_t pow1  = seq_pow((seq_t)d,  k);
                    wk += mult1 * pow1;
                    if (d2 != d) {
                        /* paired divisor d2 */
                        seq_t mult2 = fact[k] / fact[d];
                        seq_t pow2  = seq_pow((seq_t)d2, k);
                        wk += mult2 * pow2;
                    }
                }
            }
            wcache[k] = wk;
        }
        w_max = n;
    }

    /* --- compute a[i] for i up to n ------------------------------------- */
    if (acache == NULL) {
        acache = (seq_t *)malloc(sizeof(seq_t));
        acap   = 1;
        acache[0] = 1;  /* a(0) */
        amax     = 0;
    }
    if (amax < n) {
        grow_if_needed((size_t)(n + 1), &acap, &acache);
        for (idx_t i = amax + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = acache[i - k];
                term = term * binomial(i - 1, k - 1);
                term = term * wcache[k];
                sum += term;
            }
            acache[i] = sum;
        }
        amax = n;
    }

    return acache[n];
}

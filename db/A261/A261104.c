/*
 * A261104  a(0)=0; for n >= 1, a(n) = 1 + a(n-A070319(n)), where A070319(n)
 *          is the maximum value for A000005 (number of divisors) in range 1
 *          .. n.
 * https://oeis.org/A261104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9913, u16 n <= 9913, u32 n <= 9913, u64 n <= 9913,
 *            u128 n <= 9913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261104
   Formula: a(0)=0; for n≥1, a(n)=1+a(n-A070319(n)),
   where A070319(n)=max{τ(k):1≤k≤n} and τ(k) is the number of divisors of k. */
#include <stdlib.h>

/* static cache of computed values */
static seq_t  *a_cache   = NULL;   /* a values, size a_cap */
static idx_t  *tau_cache = NULL;   /* number of divisors, size a_cap */
static idx_t  *dmax_cache= NULL;   /* running maximum of tau, size a_cap */
static idx_t   a_cap     = 0;

/* extend the cache so that a_cache[n] is defined for all n ≤ new_cap-1 */
static void extend(idx_t new_cap)
{
    /* allocate new tables */
    idx_t *tau = (idx_t *)calloc(new_cap, sizeof(idx_t));
    if (!tau) abort();

    for (idx_t i = 1; i < new_cap; ++i)
        for (idx_t j = i; j < new_cap; j += i)
            ++tau[j];

    idx_t *dmax = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!dmax) abort();
    dmax[0] = 0;
    for (idx_t i = 1; i < new_cap; ++i)
        dmax[i] = (tau[i] > dmax[i-1]) ? tau[i] : dmax[i-1];

    seq_t *a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!a) abort();
    a[0] = (seq_t)0;
    for (idx_t i = 1; i < new_cap; ++i) {
        idx_t sub = i - dmax[i];
        a[i] = (seq_t)1 + a[sub];
    }

    /* free old tables */
    free(a_cache);
    free(tau_cache);
    free(dmax_cache);

    /* store new tables */
    a_cache   = a;
    tau_cache = tau;
    dmax_cache= dmax;
    a_cap     = new_cap;
}

/* public interface */
seq_t a(idx_t n)
{
    if (n >= a_cap)            /* extend cache if needed */
        extend(n + 1);
    return a_cache[n];
}

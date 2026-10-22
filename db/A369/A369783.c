/*
 * A369783  Expansion of e.g.f. exp( exp(3*(exp(x)-1))-1 ).
 * https://oeis.org/A369783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} 3^k * Stirling2(n,k) * Bell(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/seq_alloc.h"  /* seq_alloc(size_t) */

/* Forward declaration of the exported function */
seq_t a(idx_t n);

/* Static arrays for memoisation */
static seq_t *a_arr    = NULL;   /* a[n]   */
static seq_t *s_arr    = NULL;   /* S(n,k) for current n */
static seq_t *bell_arr = NULL;   /* Bell(k) */
static seq_t *pow3_arr = NULL;   /* 3^k */

static size_t  cap          = 0;   /* current array capacity      */
static idx_t   computed_upto = 0;  /* highest n computed so far   */

/* Ensure the arrays have room for indices 0..n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t oldcap = cap;
        size_t newcap = needed * 2;

        seq_t *na = seq_alloc(newcap);
        seq_t *sa = seq_alloc(newcap);
        seq_t *ba = seq_alloc(newcap);
        seq_t *pa = seq_alloc(newcap);

        /* initialise new entries to zero */
        for (size_t i = 0; i < newcap; ++i) {
            na[i] = sa[i] = ba[i] = pa[i] = (seq_t)0;
        }

        /* copy old data if any */
        for (size_t i = 0; i < oldcap; ++i) {
            na[i] = a_arr[i];
            sa[i] = s_arr[i];
            ba[i] = bell_arr[i];
            pa[i] = pow3_arr[i];
        }

        a_arr    = na;
        s_arr    = sa;
        bell_arr = ba;
        pow3_arr = pa;
        cap      = newcap;

        /* initialise the very first values only once */
        if (oldcap == 0) {
            a_arr[0]    = (seq_t)1;   /* a(0) = 1 */
            s_arr[0]    = (seq_t)1;   /* S(0,0) = 1 */
            bell_arr[0] = (seq_t)1;   /* Bell(0) = 1 */
            pow3_arr[0] = (seq_t)1;   /* 3^0 = 1 */
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    ensure_capacity(n);

    if (n <= computed_upto) {
        return a_arr[(size_t)n];
    }

    /* Compute new values from (computed_upto + 1) up to n */
    for (size_t i = (size_t)computed_upto + 1; i <= (size_t)n; ++i) {
        /* Update Stirling numbers S(i,k) in place */
        for (size_t k = i; k > 0; --k) {
            seq_t term1 = s_arr[k] * (seq_t)k;   /* k * S(i-1,k) */
            seq_t term2 = s_arr[k - 1];          /* S(i-1,k-1)   */
            s_arr[k] = term1 + term2;
        }
        s_arr[0] = (seq_t)0;   /* S(i,0) = 0 for i > 0 */

        /* Compute Bell(i) via recurrence */
        seq_t bell_sum = (seq_t)0;
        seq_t comb     = (seq_t)1;                /* C(i-1,0) = 1 */
        for (size_t j = 0; j < i; ++j) {
            bell_sum += comb * bell_arr[j];
            /* Next binomial coefficient: C(i-1,j+1) */
            comb = comb * (seq_t)(i - 1 - j);
            comb = comb / (seq_t)(j + 1);
        }
        bell_arr[i] = bell_sum;

        /* Compute 3^i */
        pow3_arr[i] = pow3_arr[i - 1] * (seq_t)3;

        /* Compute a(i) = Σ_{k=0..i} S(i,k) * 3^k * Bell(k) */
        seq_t a_sum = (seq_t)0;
        for (size_t k = 0; k <= i; ++k) {
            seq_t prod = pow3_arr[k];
            prod = prod * bell_arr[k];
            a_sum += s_arr[k] * prod;
        }
        a_arr[i] = a_sum;
    }

    computed_upto = (idx_t)n;
    return a_arr[(size_t)n];
}

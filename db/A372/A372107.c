/*
 * A372107  G.f. A(x) satisfies A(x) = 1/( 1 - 4*x/(1 - x*A(x)) )^(1/2).
 * https://oeis.org/A372107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial((n+k-1)/2, k) * binomial(n-1,
 *            n-k)/(n-k+1) (OEIS A372107) The implementation below uses an
 *            equivalent recurrence that avoids fractional binomials. It keeps
 *            three convolution arrays (C2 and C3) and an auxiliary sum S2 for
 *            each index n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*--------------------------------------------------------------------
 *  Dynamic storage for the sequence values and auxiliary convolutions.
 *--------------------------------------------------------------------*/
static seq_t *a_vals = NULL;     /* a[n]          */
static seq_t *c2_vals = NULL;    /* C2[n] = Σ a[i]·a[n-i]      */
static seq_t *c3_vals = NULL;    /* C3[n] = Σ a[i]·C2[n-i]     */
static size_t allocated = 0;     /* length of allocated arrays */
static size_t computed_len = 0;  /* number of terms already computed */

/* Ensure that the arrays can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (allocated < required) {
        size_t new_cap = allocated ? allocated : 1;
        while (new_cap < required)
            new_cap <<= 1;                    /* exponential growth */

        /* grow arrays; realloc may move the memory */
        a_vals  = (seq_t *)realloc(a_vals,  new_cap * sizeof(seq_t));
        c2_vals = (seq_t *)realloc(c2_vals, new_cap * sizeof(seq_t));
        c3_vals = (seq_t *)realloc(c3_vals, new_cap * sizeof(seq_t));

        /* initialise any new elements to zero */
        for (size_t i = allocated; i < new_cap; ++i) {
            a_vals[i]  = 0;
            c2_vals[i] = 0;
            c3_vals[i] = 0;
        }
        allocated = new_cap;
    }
}

/* Compute all terms up to index n (inclusive). */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);

    /* initialise base case if this is the first call */
    if (computed_len == 0) {
        a_vals[0]   = 1;
        c2_vals[0]  = 1;      /* a0 · a0 */
        c3_vals[0]  = 1;      /* a0³      */
        computed_len = 1;
    }

    /* extend the precomputed region */
    for (size_t m = computed_len; m <= (size_t)n; ++m) {
        /* S2[m] = Σ_{i=1}^{m-1} a[i] · a[m-i]   (convolution ignoring i=0, m) */
        seq_t S2 = 0;
        for (size_t i = 1; i < m; ++i)
            S2 += a_vals[i] * a_vals[m - i];

        /* a[m] = ( C3[m-1] + 4·C2[m-1] - S2 - a[m-1] ) / 2  */
        seq_t numerator = c3_vals[m-1] + (seq_t)4 * c2_vals[m-1] - S2 - a_vals[m-1];
        a_vals[m] = numerator / 2;

        /* C2[m] = 2·a[m] + S2   (full convolution) */
        c2_vals[m] = (seq_t)2 * a_vals[m] + S2;

        /* C3[m] = Σ_{k=0}^{m} a[k] · C2[m-k]   (convolution of a with C2) */
        seq_t C3sum = 0;
        for (size_t k = 0; k <= m; ++k)
            C3sum += a_vals[k] * c2_vals[m - k];
        c3_vals[m] = C3sum;
    }

    computed_len = (size_t)n + 1;
}

/*--------------------------------------------------------------------
 *  External function required by the build system.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_vals[(size_t)n];
}

/*
 * A384171  a(n) = 2^n * n! * binomial(5*n/2,n) * Sum_{k=1..n} 1/(3*n+2*k).
 * https://oeis.org/A384171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n! * binomial(5*n/2,n) * Sum_{k=1..n} 1/(3*n+2*k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static seq_t *pref = NULL;
static seq_t *suf  = NULL;
static size_t pref_cap = 0;

/* Compute A384171: a(n) with offset 1 (n>=1) */
seq_t a(idx_t n)
{
    /* Ensure the product array is large enough (n+1 for prefix/suffix). */
    if (pref_cap < (size_t)(n + 1)) {
        if (pref) free(pref);
        if (suf)  free(suf);
        pref_cap = (size_t)(n + 1);
        pref = (seq_t *)malloc(pref_cap * sizeof(seq_t));
        suf  = (seq_t *)malloc(pref_cap * sizeof(seq_t));
    }

    /* Prefix products: pref[i] = product of first i denominators (k=1..i). */
    pref[0] = 1;
    for (idx_t i = 0; i < n; ++i) {
        /* denominator for k = i+1 : d = 3*n + 2*(i+1) */
        seq_t denom = (seq_t)3 * (seq_t)n + (seq_t)2 * (seq_t)(i + 1);
        pref[i + 1] = pref[i] * denom;
    }

    /* Suffix products: suf[i] = product of denominators from k=i+1 .. n. */
    suf[n] = 1;
    for (idx_t i = n; i > 0; ) {
        --i;
        seq_t denom = (seq_t)3 * (seq_t)n + (seq_t)2 * (seq_t)(i + 1);
        suf[i] = denom * suf[i + 1];
    }

    /* Sum of products of all denominators except the current one. */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t term = pref[i] * suf[i + 1];
        sum += term;
    }

    return sum;
}

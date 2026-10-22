/*
 * A355093  E.g.f. A(x) satisfies A(x) = 1 + 2 * (1 - exp(-x)) * A(1 -
 *          exp(-x)).
 * https://oeis.org/A355093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A355093
   Recurrence:
       a(0) = 1
       a(n) = 2 * Σ_{k=1..n} (-1)^{n-k} * k * S2(n,k) * a(k-1)
   where S2(n,k) are Stirling numbers of the second kind.
   Implemented with memoisation of a(n) and a single array of
   current Stirling numbers.  All arithmetic is done in seq_t.
*/

#include <stdlib.h>
#include <stddef.h>

/* remember: (seq_t, idx_t) are defined by the build */
static seq_t *a_cache = NULL;   /* cached values of a(n) */
static seq_t *S = NULL;        /* array of Stirling numbers S(n,k) for current n */
static size_t cache_cap = 0;    /* allocated size of a_cache */
static size_t S_cap = 0;        /* allocated size of S array */
static idx_t max_computed = 0;

/* Ensure the cache can hold indices up to n. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        cache_cap = new_cap;
        if (max_computed == 0) {
            a_cache[0] = (seq_t)1;  /* a(0) = 1 */
        }
    }
}

/* Ensure the S array can hold indices up to n. */
static void ensure_S_capacity(idx_t n)
{
    if ((size_t)n >= S_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        S = tmp;
        S_cap = new_cap;
        /* initialise new entries to zero */
        for (size_t i = cache_cap; i < new_cap; ++i) S[i] = (seq_t)0;
    }
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;  /* a(0) is defined */

    ensure_cache_capacity(n);
    ensure_S_capacity(n);

    /* Initialise storage for Stirling numbers if first call */
    if (cache_cap == 0) {       /* only on first call */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)0;  /* will be set when n>=1 */
        S[0] = (seq_t)0;
        S[1] = (seq_t)0;
        max_computed = 0;
    }

    /* Compute values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Update Stirling numbers for row i */
        if (i == 1) {
            S[1] = (seq_t)1;                     /* S(1,1) */
        } else {
            for (idx_t k = i; k >= 1; --k) {
                /* S(k) = k * old_S(k) + old_S(k-1) */
                seq_t tmp = S[k] * (seq_t)k;
                tmp += S[k - 1];
                S[k] = tmp;
            }
        }

        /* Compute the recurrence sum */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = S[k];          /* S(i,k) */
            term = term * (seq_t)k;     /* k * S(i,k) */
            term = term * a_cache[k - 1];/* * a(k-1) */
            if ((i - k) & 1)            /* (-1)^{i-k} */
                term = -term;
            sum += term;
        }

        a_cache[i] = sum * 2; /* a(i) = 2 * sum */
    }

    max_computed = n;
    return a_cache[n];
}

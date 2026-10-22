/*
 * A377705  G.f. A(x) satisfies A(x) = 1 + x/A(x)^2 * (1 - A(x) + A(x)^3).
 * https://oeis.org/A377705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n} (-1)^k * binomial(n,k) * *
 *            binomial(n-3*k, n-k-1) * Initial values: a(0) = 1, a(1) = 1 * *
 *            Recurrence derived from the generating function: * For n >= 1 *
 *            a[n+1] = -a[n] + C[n] - Sum_{k=0}^{n-1} a[k+1] * B[n-k] * where
 *            * B[n] = Sum_{i=0..n} a[i] * a[n-i] (A(x)^2) * C[n...
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 19, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The integer types seq_t and idx_t are defined by the build.  */

static seq_t *a_vals = NULL;          /* a[n] values   */
static seq_t *B_vals = NULL;          /* coefficients of A(x)^2 */
static seq_t *C_vals = NULL;          /* coefficients of A(x)^3 */
static size_t cache_cap = 0;          /* current allocation capacity */
static idx_t  cache_len = 0;          /* number of values already computed (0..cache_len-1) */

/* Ensure the arrays can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Reallocate a_vals */
    seq_t *p = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!p) abort();
    a_vals = p;

    /* Reallocate B_vals */
    p = (seq_t *)realloc(B_vals, new_cap * sizeof(seq_t));
    if (!p) abort();
    B_vals = p;

    /* Reallocate C_vals */
    p = (seq_t *)realloc(C_vals, new_cap * sizeof(seq_t));
    if (!p) abort();
    C_vals = p;

    /* Initialise newly allocated entries to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_vals[i] = (seq_t)0;
        B_vals[i] = (seq_t)0;
        C_vals[i] = (seq_t)0;
    }

    cache_cap = new_cap;
}

/* Primary sequence function */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return a_vals[n];

    ensure_cache(n);

    /* Compute values up to n, inclusive */
    for (idx_t m = cache_len; m <= n; ++m) {
        if (m == 0) {                     /* a(0) = 1 */
            a_vals[0] = (seq_t)1;
            B_vals[0] = (seq_t)1;         /* a[0]^2 */
            C_vals[0] = (seq_t)1;         /* a[0]^3 */
            continue;
        }

        if (m == 1) {                     /* a(1) = 1 */
            a_vals[1] = (seq_t)1;
            /* B[1] = a0*a1 + a1*a0 = 2 */
            B_vals[1] = (seq_t)2;
            /* C[1] = a0*B1 + a1*B0 = 1*2 + 1*1 = 3 */
            C_vals[1] = (seq_t)3;
            continue;
        }

        /* m >= 2 : compute a[m] first using the recurrence */
        idx_t prev = m - 1;                      /* n in the recurrence */
        seq_t sum1 = (seq_t)0;

        for (idx_t k = 0; k <= prev - 1; ++k)    /* sum_{k=0}^{n-1} a[k+1] * B[n-k] */
            sum1 += a_vals[k + 1] * B_vals[prev - k];

        /* a[m] = -a[prev] + C[prev] - sum1 */
        a_vals[m] = -a_vals[prev] + C_vals[prev] - sum1;

        /* Now compute B[m] = Sum_{i=0..m} a[i] * a[m-i] */
        seq_t sumB = (seq_t)0;
        for (idx_t i = 0; i <= m; ++i)
            sumB += a_vals[i] * a_vals[m - i];
        B_vals[m] = sumB;

        /* Compute C[m] = Sum_{i=0..m} a[i] * B[m-i] */
        seq_t sumC = (seq_t)0;
        for (idx_t i = 0; i <= m; ++i)
            sumC += a_vals[i] * B_vals[m - i];
        C_vals[m] = sumC;
    }

    cache_len = n + 1;      /* we have computed up to index n */
    return a_vals[n];
}

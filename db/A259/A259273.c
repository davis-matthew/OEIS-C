/*
 * A259273  G.f.: A(x) = exp( Sum_{n>=1} 3^n * x^n/(n*(1+x^n)) ).
 * https://oeis.org/A259273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x)=exp(∑_{n≥1} 3^n*x^n/(n*(1+x^n))) → a(n) = coefficient of
 *            x^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 1
 *   n*a(n) = Σ_{k=1}^{n} b_k * a(n-k)          (n≥1)
 *   b_k   = Σ_{d|k} 3^d * (k/d) * (-1)^{k/d-1}  (k≥1)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for memoisation */
static seq_t *pow3_arr = NULL;
static seq_t *b_arr    = NULL;
static seq_t *a_arr    = NULL;
static size_t cap = 0;      /* maximum n for which we have computed a and b */

/* precompute powers of 3, coeffs b[1..N], and a[0..N] */
static void ensure_precomp(idx_t N)
{
    /* Allocate if we have never allocated or need more memory.  We also
     * need an allocation when N == 0 because a(0) must be defined. */
    if (N <= cap && cap > 0)
        return;

    /* free old buffers if they exist */
    if (pow3_arr) free(pow3_arr);
    if (b_arr)    free(b_arr);
    if (a_arr)    free(a_arr);

    /* allocate new buffers of size N+1 (indices 0..N) */
    pow3_arr = (seq_t *)malloc((size_t)(N + 1u) * sizeof(seq_t));
    b_arr    = (seq_t *)malloc((size_t)(N + 1u) * sizeof(seq_t));
    a_arr    = (seq_t *)malloc((size_t)(N + 1u) * sizeof(seq_t));

    if (!pow3_arr || !b_arr || !a_arr) {   /* allocation failed */
        /* nothing more we can do – leave uninitialised */
        pow3_arr = b_arr = a_arr = NULL;
        cap = 0;
        return;
    }

    /* initialise to zero – safe for any seq_t implementation */
    for (size_t i = 0; i <= (size_t)N; ++i) {
        pow3_arr[i] = 0;
        b_arr[i]    = 0;
        a_arr[i]    = 0;
    }

    /* powers of 3 */
    pow3_arr[0] = (seq_t)1;
    for (size_t d = 1; d <= (size_t)N; ++d) {
        pow3_arr[d] = pow3_arr[d - 1] * (seq_t)3;
    }

    /* temporary arrays for positive and negative parts of b_k   */
    seq_t *pos_arr = (seq_t *)malloc((size_t)(N + 1u) * sizeof(seq_t));
    seq_t *neg_arr = (seq_t *)malloc((size_t)(N + 1u) * sizeof(seq_t));
    if (!pos_arr || !neg_arr) {
        free(pow3_arr);
        free(b_arr);
        free(a_arr);
        pow3_arr = b_arr = a_arr = NULL;
        if (pos_arr) free(pos_arr);
        if (neg_arr) free(neg_arr);
        cap = 0;
        return;
    }

    for (size_t i = 0; i <= (size_t)N; ++i) {
        pos_arr[i] = 0;
        neg_arr[i] = 0;
    }

    /* compute b_k using the divisor sum */
    for (size_t d = 1; d <= (size_t)N; ++d) {
        seq_t pow3 = pow3_arr[d];
        size_t max_t = (size_t)N / d;
        for (size_t t = 1; t <= max_t; ++t) {
            size_t k = d * t;                 /* k = d*t */
            seq_t term = pow3 * (seq_t)t;     /* 3^d * t */

            if ((t & 1u) == 0u)               /* t even → negative */
                neg_arr[k] += term;
            else                              /* t odd  → positive */
                pos_arr[k] += term;
        }
    }

    /* combine positive and negative parts */
    for (size_t k = 1; k <= (size_t)N; ++k) {
        b_arr[k] = pos_arr[k];
        b_arr[k] -= neg_arr[k];
    }

    free(pos_arr);
    free(neg_arr);

    /* recurrence for a(n) */
    a_arr[0] = (seq_t)1;
    for (size_t n = 1; n <= (size_t)N; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) {
            sum += b_arr[k] * a_arr[n - k];
        }
        a_arr[n] = sum / (seq_t)n;   /* exact division */
    }

    cap = (size_t)N;
}

seq_t a(idx_t n)
{
    ensure_precomp(n);
    return a_arr[(size_t)n];
}

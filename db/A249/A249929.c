/*
 * A249929  G.f. A(x) satisfies: 1+x = A(x)^3 + A(x)^4 - A(x)^6.
 * https://oeis.org/A249929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y(n) where y(x)=A(x)-1 satisfies y = x + 6 y² + 15 y³ +
 *            14 y⁴ + 6 y⁵ + y⁶. a(0)=1, a(n)=y(n) for n≥1. Recurrence: for
 *            n≥2 y(n) = 6·C2[n] + 15·C3[n] + 14·C4[n] + 6·C5[n] + 1·C6[n]
 *            where Ck[n] = Σ_{i1+…+ik=n, ij≥1} y(i1)…y(ik) (k‑fold
 *            convolution, all indices < n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *y_arr     = NULL;   /* y(0) unused, y[1] = 1                     */
static seq_t *c2_arr    = NULL;   /* convolution of y with itself              */
static seq_t *c3_arr    = NULL;   /* convolution of y with c2                  */
static seq_t *c4_arr    = NULL;   /* convolution of y with c3                  */
static seq_t *c5_arr    = NULL;   /* convolution of y with c4                  */
static seq_t *c6_arr    = NULL;   /* convolution of y with c5                  */
static idx_t  max_n     = 0;       /* largest n that has been computed          */

/* Ensure all arrays have room for indices up to n (inclusive). */
static void
ensure_capacity(idx_t n)
{
    if ((max_n >= n) && y_arr) return;

    /* Decide new capacity: doubling strategy, at least N+1. */
    idx_t new_cap = (max_n == 0 ? 32 : max_n * 2);
    while (new_cap < n + 1) new_cap *= 2;

#ifdef __cplusplus
    seq_t *new_y     = new seq_t[new_cap];
    seq_t *new_c2    = new seq_t[new_cap];
    seq_t *new_c3    = new seq_t[new_cap];
    seq_t *new_c4    = new seq_t[new_cap];
    seq_t *new_c5    = new seq_t[new_cap];
    seq_t *new_c6    = new seq_t[new_cap];
    if (y_arr) {                    /* copy old data */
        for (idx_t i = 0; i <= max_n; ++i) {
            new_y[i]  = y_arr[i];
            new_c2[i] = c2_arr[i];
            new_c3[i] = c3_arr[i];
            new_c4[i] = c4_arr[i];
            new_c5[i] = c5_arr[i];
            new_c6[i] = c6_arr[i];
        }
    }
    if (y_arr) {
        delete [] y_arr;
        delete [] c2_arr;
        delete [] c3_arr;
        delete [] c4_arr;
        delete [] c5_arr;
        delete [] c6_arr;
    }
    y_arr  = new_y;
    c2_arr = new_c2;
    c3_arr = new_c3;
    c4_arr = new_c4;
    c5_arr = new_c5;
    c6_arr = new_c6;
#else
    y_arr  = realloc(y_arr,  new_cap * sizeof(seq_t));
    c2_arr = realloc(c2_arr, new_cap * sizeof(seq_t));
    c3_arr = realloc(c3_arr, new_cap * sizeof(seq_t));
    c4_arr = realloc(c4_arr, new_cap * sizeof(seq_t));
    c5_arr = realloc(c5_arr, new_cap * sizeof(seq_t));
    c6_arr = realloc(c6_arr, new_cap * sizeof(seq_t));
    if (!y_arr || !c2_arr || !c3_arr || !c4_arr || !c5_arr || !c6_arr) abort();
#endif

    /* Zero the new slots. */
    for (idx_t i = max_n + 1; i < new_cap; ++i) {
        y_arr[i]  = 0;
        c2_arr[i] = 0;
        c3_arr[i] = 0;
        c4_arr[i] = 0;
        c5_arr[i] = 0;
        c6_arr[i] = 0;
    }

    max_n = new_cap - 1;

    /* Initialise base cases if this is the first call. */
    if (max_n >= 1 && y_arr[1] == 0) {
        y_arr[1] = 1;          /* y(1) = 1  (a(1)=1) */
    }
}

/* Main sequence accessor. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0)=1 */

    ensure_capacity(n);

    /* Compute all missing terms up to n. */
    for (idx_t m = (max_n >= 1 ? 2u : 2u); m <= n; ++m) {
        /* convolution c2[m] = Σ_{i=1}^{m-1} y[i] * y[m-i] */
        seq_t sum = 0;
        for (idx_t i = 1; i < m; ++i) {
            sum += y_arr[i] * y_arr[m - i];
        }
        c2_arr[m] = sum;

        /* convolution c3[m] = Σ_{i=1}^{m-1} y[i] * c2[m-i] */
        seq_t sum3 = 0;
        for (idx_t i = 1; i < m; ++i) {
            sum3 += y_arr[i] * c2_arr[m - i];
        }
        c3_arr[m] = sum3;

        /* convolution c4[m] = Σ_{i=1}^{m-1} y[i] * c3[m-i] */
        seq_t sum4 = 0;
        for (idx_t i = 1; i < m; ++i) {
            sum4 += y_arr[i] * c3_arr[m - i];
        }
        c4_arr[m] = sum4;

        /* convolution c5[m] = Σ_{i=1}^{m-1} y[i] * c4[m-i] */
        seq_t sum5 = 0;
        for (idx_t i = 1; i < m; ++i) {
            sum5 += y_arr[i] * c4_arr[m - i];
        }
        c5_arr[m] = sum5;

        /* convolution c6[m] = Σ_{i=1}^{m-1} y[i] * c5[m-i] */
        seq_t sum6 = 0;
        for (idx_t i = 1; i < m; ++i) {
            sum6 += y_arr[i] * c5_arr[m - i];
        }
        c6_arr[m] = sum6;

        /* y[m] = 6*c2 + 15*c3 + 14*c4 + 6*c5 + 1*c6 */
        y_arr[m] =   (seq_t)6 * c2_arr[m]
                   + (seq_t)15 * c3_arr[m]
                   + (seq_t)14 * c4_arr[m]
                   + (seq_t)6  * c5_arr[m]
                   + c6_arr[m];
    }

    max_n = n;   /* We now have all terms up to n computed. */

    return y_arr[n];   /* a(n) = y(n) for n≥1 */
}

/*
 * A249926  G.f. A(x) satisfies: 1+x = A(x)^2 + A(x)^4 - A(x)^5.
 * https://oeis.org/A249926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from 1 + x = A(x)^2 + A(x)^4 – A(x)^5
   (OEIS A249926).  The series A(x) = 1 + Σ_{n≥1} a(n)x^n, a(0)=1.
   For n≥1:
       a(n) = (n==1) + 3·C2(n) + 6·C3(n) + 4·C4(n) + C5(n),
   where Ck(n) is the coefficient of x^n in (A(x) – 1)^k
   computed from the already known a(i), i<n.
   The implementation keeps these convolution tables
   incrementally for up to the required index.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper arrays and state */
static seq_t *g  = NULL;  /* g[n] = a(n), for n≥1  */
static seq_t *h2 = NULL; /* h2[n] = coefficient of x^n in (A-1)^2 */
static seq_t *h3 = NULL; /* h3[n] = coefficient of x^n in (A-1)^3 */
static seq_t *h4 = NULL; /* h4[n] = coefficient of x^n in (A-1)^4 */
static seq_t *h5 = NULL; /* h5[n] = coefficient of x^n in (A-1)^5 */
static idx_t  max_cached = 0;

/* ensure capacity for arrays up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= max_cached) return;
    size_t new_cap = n + 1;
    if (g == NULL) { /* first allocation */
        g  = (seq_t *)calloc(new_cap, sizeof(seq_t));
        h2 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        h3 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        h4 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        h5 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        /* g[0] stays 0; arrays are zeroed */
    } else {
        g  = (seq_t *)realloc(g,  new_cap * sizeof(seq_t));
        h2 = (seq_t *)realloc(h2, new_cap * sizeof(seq_t));
        h3 = (seq_t *)realloc(h3, new_cap * sizeof(seq_t));
        h4 = (seq_t *)realloc(h4, new_cap * sizeof(seq_t));
        h5 = (seq_t *)realloc(h5, new_cap * sizeof(seq_t));
    }
    /* compute new values from max_cached+1 .. n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        /* h2[k] = Σ_{i=1}^{k-1} g[i] * g[k-i] */
        seq_t sum2 = 0;
        for (idx_t i = 1; i < k; ++i) sum2 += g[i] * g[k - i];
        h2[k] = sum2;

        /* h3[k] = Σ_{i=1}^{k-1} g[i] * h2[k-i] */
        seq_t sum3 = 0;
        for (idx_t i = 1; i < k; ++i) sum3 += g[i] * h2[k - i];
        h3[k] = sum3;

        /* h4[k] = Σ_{i=1}^{k-1} g[i] * h3[k-i] */
        seq_t sum4 = 0;
        for (idx_t i = 1; i < k; ++i) sum4 += g[i] * h3[k - i];
        h4[k] = sum4;

        /* h5[k] = Σ_{i=1}^{k-1} g[i] * h4[k-i] */
        seq_t sum5 = 0;
        for (idx_t i = 1; i < k; ++i) sum5 += g[i] * h4[k - i];
        h5[k] = sum5;

        /* g[k] = (k==1) + 3*h2[k] + 6*h3[k] + 4*h4[k] + h5[k] */
        seq_t gk = (k == 1) ? (seq_t)1 : (seq_t)0;
        gk += (seq_t)3 * h2[k] + (seq_t)6 * h3[k] + (seq_t)4 * h4[k] + h5[k];
        g[k] = gk;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    ensure_capacity(n);
    return g[n];                           /* a(n) = g[n] for n≥1 */
}

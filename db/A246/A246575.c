/*
 * A246575  Expansion of (Product_{r>=1} (1-x^r))*x^(k^2) / Product_{i=1..k}
 *          (1-x^i)^2 with k=1.
 * https://oeis.org/A246575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{k=0}^{n-1} b_k * (n-k) where b_k are the
 *            coefficients of ∏_{r≥1}(1-x^r) (Euler's pentagonal theorem: b_0
 *            = 1, for k>0, b_k = ±1 if k is a generalized pentagonal number,
 *            otherwise b_k = 0)
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The array a is built incrementally and memoised on use. */

#include <stdlib.h>

static seq_t *a_vals = NULL;      /* a[0..cap-1]  */
static size_t a_cap = 0;
static seq_t *b_vals = NULL;      /* coeffs of ∏(1-x^r) beginning at x^0 */
static size_t b_cap = 0;
static seq_t sumB = 0;            /* Σ_{j=0}^{m-1} b_j  for current m  */
static seq_t sumK = 0;            /* Σ_{j=0}^{m-1} j*b_j */
static idx_t last_computed = 0;   /* highest index already stored in a_vals */

/* initialise or enlarge the cached arrays up to at least n steps */
static void ensure_cache(idx_t n)
{
    if (n <= last_computed)    /* nothing to do */
        return;

    /* grow capacity to a power of two >= n+1 */
    size_t new_cap = a_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    b_vals = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
    a_cap = b_cap = new_cap;

    /* recompute all b[k] for k<new_cap from scratch */
    for (size_t i = 0; i < new_cap; ++i)
        b_vals[i] = 0;
    if (new_cap > 0)
        b_vals[0] = 1;          /* coefficient of x^0 in ∏(1-x^r) is 1 */

    /* Generate all generalized pentagonal numbers ≤ new_cap-1 */
    for (idx_t k = 1; ; ++k) {
        idx_t g1 = k * (3 * k - 1) / 2;
        idx_t g2 = k * (3 * k + 1) / 2;
        seq_t sign = (k % 2 == 0) ? 1 : -1;   /* (-1)^k */

        if (g1 < (idx_t)new_cap) b_vals[g1] = sign;
        if (g2 < (idx_t)new_cap) b_vals[g2] = sign;

        if (g1 >= (idx_t)new_cap && g2 >= (idx_t)new_cap)
            break;
    }

    /* Re‑compute sums and a[n] only for the new indices. */
    /* If this is the first time, the sums are zero because nothing
       contributed before index 0.  They stay correct after the
       re‑initialisation of b_vals because the values do not change. */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        seq_t bm1 = b_vals[m - 1];
        sumB += bm1;
        sumK += (seq_t)(m - 1) * bm1;
        a_vals[m] = (seq_t)m * sumB - sumK;
    }
    last_computed = n;
}

/* Public function implementing the sequence for any n ≥ 0 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* offset 0: a(0)=0 */
    ensure_cache(n);
    return a_vals[n];
}

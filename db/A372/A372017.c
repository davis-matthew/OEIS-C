/*
 * A372017  G.f. A(x) satisfies A(x) = ( 1 + 9*x/(1 - x*A(x)) )^(1/3).
 * https://oeis.org/A372017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) computed from the functional equation * A(x)^3 = 1 + 9*x/(1
 *            - x*A(x)) * which yields the recurrence * a(0) = 1 * a(n) = (
 *            C4(n-1) - a(n-1) + 9*δ_{n,1} * - Σ_{i=1}^{n-1} a(i) * C2(n-i) *
 *            - Σ_{j=1}^{n-1} a(j) * a(n-j) ) / 3 * where * C2(m) = Σ_{i+j=m}
 *            a(i) * a(j) * C4(m) = Σ_{i+j=m} C2(i) * C2(j)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Allocation helpers for the cached sequences */
static seq_t *a_array = NULL;      /* a(n) values */
static seq_t *c2_array = NULL;    /* convolution of a with a */
static seq_t *c4_array = NULL;    /* convolution of c2 with c2 */
static size_t cache_size = 0;      /* number of terms currently stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure that we have space for terms 0..n (inclusive) */
static void ensure_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = needed;
    seq_t *na = (seq_t *)realloc(a_array, new_cap * sizeof(seq_t));
    seq_t *nc2 = (seq_t *)realloc(c2_array, new_cap * sizeof(seq_t));
    seq_t *nc4 = (seq_t *)realloc(c4_array, new_cap * sizeof(seq_t));
    if (!na || !nc2 || !nc4) {
        /* allocation failed – abort */
        abort();
    }
    a_array = na;
    c2_array = nc2;
    c4_array = nc4;
    cache_cap = new_cap;
}

/* Compute all terms up to n (if not already computed) */
static void compute_upto(idx_t n)
{
    if (cache_size > 0 && n < (idx_t)(cache_size - 1))
        return;          /* already computed */

    /* grow memory as needed */
    ensure_cap(n);

    /* initialize first term if necessary */
    if (cache_size == 0) {
        a_array[0] = (seq_t)1;
        c2_array[0] = (seq_t)1;      /* 1*1 */
        c4_array[0] = (seq_t)1;      /* 1*1 */
        cache_size = 1;
    }

    /* compute missing terms */
    for (idx_t m = (idx_t)cache_size; m <= n; ++m) {
        /* delta term for n == 1 */
        seq_t delta = (m == 1 ? (seq_t)9 : (seq_t)0);

        /* C4(m-1) - a(m-1) + delta */
        seq_t term = c4_array[m - 1] - a_array[m - 1] + delta;

        /* subtract Σ_{i=1}^{m-1} a[i] * C2(m-i) */
        seq_t sum_ai_c2 = (seq_t)0;
        for (idx_t i = 1; i < m; ++i)
            sum_ai_c2 += a_array[i] * c2_array[m - i];

        /* subtract Σ_{j=1}^{m-1} a[j] * a[m-j] */
        seq_t sum_aa = (seq_t)0;
        for (idx_t j = 1; j < m; ++j)
            sum_aa += a_array[j] * a_array[m - j];

        seq_t numerator = term - sum_ai_c2 - sum_aa;
        a_array[m] = numerator / (seq_t)3;

        /* update C2[m] = Σ_{k=0}^m a[k] * a[m-k] */
        seq_t c2m = (seq_t)0;
        for (idx_t k = 0; k <= m; ++k)
            c2m += a_array[k] * a_array[m - k];
        c2_array[m] = c2m;

        /* update C4[m] = Σ_{i=0}^m C2[i] * C2[m-i] */
        seq_t c4m = (seq_t)0;
        for (idx_t i = 0; i <= m; ++i)
            c4m += c2_array[i] * c2_array[m - i];
        c4_array[m] = c4m;
    }

    cache_size = (size_t)n + 1;
}

/* Public function requested by the test harness */
seq_t a(idx_t n)
{
    if (n < 0)
        return (seq_t)0;   /* undefined for negative indices */
    compute_upto(n);
    return a_array[(size_t)n];
}

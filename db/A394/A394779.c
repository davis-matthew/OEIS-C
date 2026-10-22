/*
 * A394779  G.f. A(x) satisfies A(x) = 1 - 3*x * d/dx log(1 - x*A(x)^2).
 * https://oeis.org/A394779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A394779
 * 
 * Recurrence (provided by the OEIS entry):
 *   a(0) = 1
 *   a(n) = (3*n - 1) * Σ_{k=0}^{n-1} a(k) * a(n-1-k)
 *          + Σ_{i,j,k≥0 , i+j+k = n-1} a(i) * a(j) * a(k)
 *
 * The implementation keeps the sequence values and two convolution
 * tables:
 *   conv[t] = Σ_{i=0}^{t} a(i) * a(t-i)         (pairwise convolution)
 *   It is updated incrementally when a new a(m) is computed.
 *   The triple convolution is evaluated on the fly as
 *   T_m = Σ_{k=0}^{m-1} a(k) * conv[m-1-k].
 *
 * Complexity:  O(n²) arithmetic operations per sequence up to n.
 * The routine works in any build: C11 or C++17, with arbitrary
 * precision or fixed-width integer types for seq_t.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/grow_arr.h"

/* memoised array of sequence values (a(0), a(1), ...) */
static seq_t *a_cache = NULL;
/* current number of stored values == capacity since we add in order */
static size_t a_len = 0;

/* pairwise convolution array: conv[t] = Σ a[i]*a[t-i] */
static seq_t *conv = NULL;
/* current capacity of conv */
static size_t conv_len = 0;

/* public interface */
seq_t a(idx_t n)
{
    /* initialise a(0) and conv[0] on first call */
    if (a_len == 0) {
        a_len = 1;
        a_cache = (seq_t *)realloc(a_cache, a_len * sizeof(seq_t));
        a_cache[0] = (seq_t)1;

        conv_len = 1;
        conv = (seq_t *)realloc(conv, conv_len * sizeof(seq_t));
        conv[0] = (seq_t)1;   /* a[0] * a[0] */
    }

    /* extend memoisation up to n (in case calls are out of order) */
    for (idx_t m = a_len; m <= n; ++m) {
        /* single–pair convolution for m-1: C = conv[m-1] */
        seq_t C = conv[(size_t)(m - 1)];

        /* triple convolution for m-1: T = Σ_{k=0}^{m-1} a(k) * conv[m-1-k] */
        seq_t T = 0;
        for (idx_t k = 0; k < m; ++k)
            T += a_cache[(size_t)k] * conv[(size_t)((m - 1) - k)];

        /* a[m] = (3*m - 1)*C + T */
        seq_t factor = ((seq_t)3 * m) - 1;
        seq_t a_val = factor * C + T;

        /* store a[m] */
        grow_arr(&a_cache, &a_len, (size_t)m + 1);
        a_cache[(size_t)m] = a_val;
        a_len = (size_t)m + 1;   /* update number of stored values */

        /* update conv for indices t = m .. 2*m */
        /* we need capacity up to 2*m (inclusive) */
        size_t need_conv = 2 * (size_t)m;
        grow_arr(&conv, &conv_len, need_conv + 1);

        /* incremental convolution update:
           conv[t] += Σ_{i=0}^{m} a[i] * a[m]    (t = i + m)
           with each product counted twice unless i == m */
        for (idx_t i = 0; i <= m; ++i) {
            size_t t = i + (size_t)m;
            seq_t prod = a_cache[(size_t)i] * a_val;
            if (i == m)
                conv[t] += prod;          /* double‑counted term a[m]*a[m] once */
            else
                conv[t] += prod + prod;   /* add 2*a[i]*a[m] */
        }
    }

    return a_cache[(size_t)n];
}

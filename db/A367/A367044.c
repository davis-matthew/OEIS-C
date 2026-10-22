/*
 * A367044  G.f. satisfies A(x) = 1 - x^2 + x*A(x)^3.
 * https://oeis.org/A367044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the functional equation 
 *    A(x) = 1 - x^2 + x*A(x)^3.
 * The coefficient a(n) can be computed iteratively by
 *    a(0) = 1, 
 *    for n ≥ 1 : a(n) =  Σ_{i+j+k = n-1} a(i)·a(j)·a(k)  –  [n == 2].
 * This implementation builds a[0…n] incrementally using the convolution
 * seen in A(x)^2 and A(x)^3, requiring only O(n²) operations which is
 * easily fast enough for n up to about 10 000.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cached coefficients of the series A(x). */
static seq_t *a_arr   = NULL; /* a(n) */
static seq_t *e_arr   = NULL; /* coefficient of x^n in A(x)^2 */
static seq_t *c_arr   = NULL; /* coefficient of x^n in A(x)^3 */
static idx_t  cur_sz  = 0;     /* number of terms already computed */
static size_t cap     = 0;     /* allocated array capacity */

/* Expand the cached tables up to index n (inclusive). */
static void expand_to(idx_t n)
{
    if (n < cur_sz) return;          /* already computed */

    if (cur_sz == 0) {               /* first allocation */
        cap = (n + 1 > 4) ? (n + 1) : 4;
        a_arr = seq_alloc(cap);
        e_arr = seq_alloc(cap);
        c_arr = seq_alloc(cap);

        a_arr[0] = 1;                /* a(0) = 1          */
        e_arr[0] = a_arr[0] * a_arr[0];   /* A^2 coefficient 0 */
        c_arr[0] = a_arr[0] * e_arr[0];   /* A^3 coefficient 0 */
        cur_sz = 1;
    } else if (cap <= n) {           /* need to grow */
        cap = n + 1;
        a_arr = seq_realloc(a_arr, cur_sz, cap);
        e_arr = seq_realloc(e_arr, cur_sz, cap);
        c_arr = seq_realloc(c_arr, cur_sz, cap);
    }

    for (idx_t m = cur_sz; m <= n; ++m) {
        idx_t limit = m - 1;          /* degree for A^2 and A^3 */

        /* coefficient of x^limit in A(x)^2 */
        seq_t sum_e = 0;
        for (idx_t i = 0; i <= limit; ++i)
            sum_e += a_arr[i] * a_arr[limit - i];
        e_arr[limit] = sum_e;

        /* coefficient of x^limit in A(x)^3 */
        seq_t sum_c = 0;
        for (idx_t i = 0; i <= limit; ++i)
            sum_c += a_arr[i] * e_arr[limit - i];
        c_arr[limit] = sum_c;

        /* compute a(m)  */
        seq_t val = sum_c;
        if (m == 2) val -= 1;        /* minus the -x^2 term */
        a_arr[m] = val;
    }

    cur_sz = n + 1;
}

seq_t a(idx_t n)
{
    if (n >= cur_sz)
        expand_to(n);
    return a_arr[n];
}

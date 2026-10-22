/*
 * A366436  G.f. A(x) satisfies A(x) = 1 + x * ((1 - x) / A(x))^3.
 * https://oeis.org/A366436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=0..n} binomial(4*k-1,k) *
 *            binomial(3*k,n-k) / (4*k-1)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_arr    = NULL;
static seq_t *b2_arr   = NULL;
static seq_t *b3_arr   = NULL;
static size_t cache_sz = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow cache to at least n+1 elements */
    if (cache_sz <= n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_a  = (seq_t*)realloc(a_arr, new_sz * sizeof(seq_t));
        seq_t *new_b2 = (seq_t*)realloc(b2_arr, new_sz * sizeof(seq_t));
        seq_t *new_b3 = (seq_t*)realloc(b3_arr, new_sz * sizeof(seq_t));
        if (!new_a || !new_b2 || !new_b3) {
            /* Allocation failed – in this environment we simply abort. */
            abort();
        }
        a_arr  = new_a;
        b2_arr = new_b2;
        b3_arr = new_b3;

        /* Initialize the first element(s) if cache was empty */
        if (cache_sz == 0) {
            a_arr[0]  = (seq_t)1;   /* a0 = 1 */
            b2_arr[0] = (seq_t)1;   /* a0*a0 */
            b3_arr[0] = (seq_t)1;   /* a0*a0*a0 */
            cache_sz = 1;
        }
    }

    /* Extend computation up to n if necessary */
    for (idx_t i = (idx_t)cache_sz; i <= n; ++i) {
        seq_t K2 = (seq_t)0; /* sum_{k=1}^{i-1} a[k]*a[i-k]          */
        seq_t Y  = (seq_t)0; /* sum_{k=1}^{i-1} a[k]*b2[i-k]         */
        seq_t Z  = (seq_t)0; /* sum_{k=1}^{i-1} a[k]*b3[i-k]         */
        for (idx_t k = (idx_t)1; k < i; ++k) {
            K2 += a_arr[k] * a_arr[i - k];
            Y  += a_arr[k] * b2_arr[i - k];
            Z  += a_arr[k] * b3_arr[i - k];
        }

        /* RHS coefficient of x^i in x(1-x)^3 */
        seq_t rhs;
        switch (i) {
            case 1: rhs = (seq_t)1;  break;
            case 2: rhs = (seq_t)-3; break;
            case 3: rhs = (seq_t)3;  break;
            case 4: rhs = (seq_t)-1; break;
            default: rhs = (seq_t)0; break;
        }

        /* a[i] = rhs - Z */
        seq_t an = rhs - Z;
        a_arr[i]  = an;
        b2_arr[i] = K2 + (seq_t)2 * an;
        b3_arr[i] = K2 + (seq_t)3 * an + Y;
    }

    cache_sz = (size_t)n + 1;
    return a_arr[n];
}

/*
 * A382335  Expansion of ( 1 + 4 * Sum_{k>=0} x^(2^k)/(1 - x^(2^k))^2 )^(1/2).
 * https://oeis.org/A382335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   A(x) satisfies A(x) = sqrt( A(x^2)^2 + 4*x/(1-x)^2 ). Let c[n] =
 *            ∑_{k=0..n} a[k]*a[n-k]. Then c[0] = 1, for n ≥ 1 : if n odd :
 *            c[n] = 4*n if n even: c[n] = 4*n + c[n/2] Finally, for n ≥ 1
 *            a[n] = ( c[n] - ∑_{k=1..n-1} a[k]*a[n-k] ) / 2 with a[0] = 1.
 *            This recurrence generates the OEIS sequence A38...
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 24, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static void ensure_cap(size_t *cap, seq_t **arr, size_t needed) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    static seq_t *a_arr = NULL;
    static seq_t *c_arr = NULL;
    static size_t cap_a = 0, cap_c = 0;
    static idx_t computed = 0;   /* number of coefficients already filled (a[0..computed-1]) */

    /* Ensure we have space for up to n inclusive */
    size_t need = (size_t)n + 1;
    if (cap_a < need) ensure_cap(&cap_a, &a_arr, need);
    if (cap_c < need) ensure_cap(&cap_c, &c_arr, need);

    if (computed == 0) {
        a_arr[0] = (seq_t)1;
        c_arr[0] = (seq_t)1;
        computed = 1;
    }

    for (idx_t i = computed; i <= n; ++i) {
        /* compute c[i] using the recurrence */
        seq_t r = (seq_t)4 * (seq_t)i;
        seq_t ci;
        if (i & 1) {
            ci = r;
        } else {
            ci = r + c_arr[i / 2];
        }
        c_arr[i] = ci;

        /* compute the convolution sum s = ∑_{k=1}^{i-1} a[k]*a[i-k] */
        seq_t s = 0;
        for (idx_t k = 1; k < i; ++k) {
            s += a_arr[k] * a_arr[i - k];
        }

        /* solve for a[i] */
        a_arr[i] = (ci - s) / 2;
    }

    computed = n + 1;
    return a_arr[n];
}

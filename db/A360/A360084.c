/*
 * A360084  a(n) = Sum_{k=0..n} (-1)^k * binomial(3*k,n-k) * Catalan(k).
 * https://oeis.org/A360084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - ( c_{n-1} + 3*c_{n-2} + 3*c_{n-3} + c_{n-4} ), where
 *            c_m = Σ_{i=0..m} a_i * a_{m-i}. a(0)=1, c_0=1
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 25, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes the values of a(n) and the convolution
   dates c(n) incrementally, so any order of calls is supported. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static arrays for memoisation */
    static seq_t *a_arr = NULL;
    static seq_t *c_arr = NULL;
    static size_t a_cap = 0, c_cap = 0;
    static idx_t last_computed = 0; /* inclusive last index computed */

    /* ensure capacity for up to n */
    size_t need = (size_t)(n + 1);
    if (need > a_cap) {
        size_t new_cap = a_cap ? a_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
    if (need > c_cap) {
        size_t new_cap = c_cap ? c_cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;
        c_arr = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
        c_cap = new_cap;
    }

    /* initialise first elements if not yet done */
    if (last_computed == 0 && a_arr != NULL && c_arr != NULL) {
        a_arr[0] = 1;
        c_arr[0] = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* fetch needed previous c values, treating out‑of‑range as 0 */
        seq_t ci1 = (i >= 1) ? c_arr[i - 1] : 0;
        seq_t ci2 = (i >= 2) ? c_arr[i - 2] : 0;
        seq_t ci3 = (i >= 3) ? c_arr[i - 3] : 0;
        seq_t ci4 = (i >= 4) ? c_arr[i - 4] : 0;

        /* recurrence for a(i) */
        a_arr[i] = -(ci1 + 3 * ci2 + 3 * ci3 + ci4);

        /* convolution to obtain c(i) = Σ_{j=0..i} a_j * a_{i-j} */
        seq_t s = 0;
        idx_t half = i >> 1;
        for (idx_t j = 0; j <= half; ++j) {
            seq_t prod = a_arr[j] * a_arr[i - j];
            if (j == i - j)
                s += prod;          /* middle term when i even */
            else
                s += prod * 2;      /* pair of symmetric terms */
        }
        c_arr[i] = s;
    }

    last_computed = n;
    return a_arr[n];
}
